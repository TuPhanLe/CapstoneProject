<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use PhpMqtt\Client\MqttClient;
use PhpMqtt\Client\ConnectionSettings;
use App\Models\MqttGarden;
use App\Models\TimeScheduleOnOff;

class HomeController extends Controller
{
    protected $garden;
    protected $timeTable;
    protected $client;
    public function __construct()
    {
        $this->garden = new MqttGarden();
        $this->timeTable = new TimeScheduleOnOff();
        $this->client = new MqttClient('569ed0169886465b9cb99dd61e2a369c.s2.eu.hivemq.cloud', 8883, 'test-publisher', MqttClient::MQTT_3_1, null, null);
        $connectionSettings = (new ConnectionSettings)
            ->setUsername('client02')
            ->setPassword('123123Aa')
            ->setUseTls(true)
            ->setTlsSelfSignedAllowed(true)
            ->setTlsVerifyPeer(false);
        $this->client->connect($connectionSettings, true);
    }

    public function index()
    {
        return view('base.dashboard');
    }

    public function setting()
    {
        return view('page.setting');
    }

    public function setting_action()
    {
        switch ($_POST['ACTION']) {
            case 1:
                $this->client->publish('server/to/gateway', ' 00 01 ff ', MqttClient::QOS_AT_MOST_ONCE);
                break;
            case 0:
                $data = $this->converDurationToGw($_POST['DURATION']);
                print_r($data);
                $this->client->publish('server/to/gateway', $data, MqttClient::QOS_AT_MOST_ONCE);
                break;
        }
    }

    public function area_1()
    {
        $data = $this->garden->showDataFromDB(1);
        return view('page.area-1', compact('data'));
    }

    public function area_2()
    {
        $data = $this->garden->showDataFromDB(2);
        return view('page.area-2', compact('data'));
    }

    public function control_1()
    {
        $TimeSchedules = TimeScheduleOnOff::orderBy('START', 'asc')->get();
        return view('page.control-1', [
            'TimeSchedules' => $TimeSchedules,
        ]);
    }

    public function control_1_action()
    {
        switch ($_POST['STATUS']) {
            case 'ON':
                $data = [
                    'STATUS' => 'ON',
                    'NOTE' => null,
                    'START' => null,
                    'DURATION' => null,
                    'DAY' => null
                ];
                TimeScheduleOnOff::create($data);
                print_r('TURN ON LIGHT');
                $this->client->publish('server/to/gateway', ' 03 06 01 00 00 00 00 00 ', MqttClient::QOS_AT_MOST_ONCE);
                break;
            case 'OFF':
                print_r('TURN OFF LIGHT');
                $data = [
                    'STATUS' => 'OFF',
                    'NOTE' => null,
                    'START' => null,
                    'DURATION' => null,
                    'DAY' => null
                ];
                TimeScheduleOnOff::create($data);
                $this->client->publish('server/to/gateway', ' 03 06 00 00 00 00 00 00 ', MqttClient::QOS_AT_MOST_ONCE);
                break;
            case 'SCHEDULE':
                $setTime = $this->convertTimeToGw($_POST['START'], $_POST['DURATION']);
                $day = $this->checkWeek($_POST['DAY']);
                $data = [
                    'STATUS' => null,
                    'NOTE' => $_POST['NOTE'],
                    'START' => $_POST['START'],
                    'DURATION' => $_POST['DURATION'],
                    'DAY' => $day
                ];
                TimeScheduleOnOff::create($data);
                break;
        }
    }
    public function control_1_delete()
    {
        $NUMBER = $_POST['NUMBER'];
        TimeScheduleOnOff::where('NUMBER', $NUMBER)->delete();
    }

    public function control_2()
    {
        return view('page.control-2');
    }

    function checkWeek($week)
    {
        // Lặp qua từng phần tử trong mảng week
        $string = "";
        foreach ($week as $key => $value) {
            if ($value >= 1 && $value <= 8) {
                // Nếu có, in ra số $value
                switch ($value) {
                    case 1:
                        $string = $string . "Hằng ngày ";
                        break;
                    case 2:
                        $string = $string . "Thứ 2 ";
                        break;
                    case 3:
                        $string = $string . "Thứ 3 ";
                        break;
                    case 4:
                        $string = $string . "Thứ 4 ";
                        break;
                    case 5:
                        $string = $string . "Thứ 5 ";
                        break;
                    case 6:
                        $string = $string . "Thứ 6 ";
                        break;
                    case 7:
                        $string = $string . "Thứ 7 ";
                        break;
                    case 8:
                        $string = $string . "Chủ nhật ";
                        break;
                }
            }
        }
        return $string;
    }

    function convertTimeToGw($start, $duration)
    {
        $string = '';
        $hourStart = intval(substr($start, 0, 2));
        $minuStart = intval(substr($start, 3, 2));
        $duration = intval($duration);
        $hourEnd = 0;
        $minEnd = 0;
        if ($duration / 60) {
            $hourEnd = floor(($duration / 60)) + $hourStart;
            $minEnd = $minuStart + ($duration % 60);
        }
        $string = $string . ($this->converToString($hourStart));
        $string = $string . ($this->converToString($minuStart));
        $string = $string . ($this->converToString($hourEnd));
        $string = $string . ($this->converToString($minEnd));
        $string = " 03 06 00 01" . $string . " ";
        return $string;
    }

    function converDurationToGw($duration)
    {
        $string = '';
        $lenOfStr = 0;
        $duration = intval($duration) * 60;
        $string = ($this->converToString($duration));
        $lenOfStr = strlen($string) - 1;
        $string = substr($string, 1);
        while ($lenOfStr < 8) {
            $string = '0' . $string;
            $lenOfStr++;
        }
        $string = substr($string, 0, 2) . ' ' . substr($string, 2);
        $string = substr($string, 0, 5) . ' ' . substr($string, 5);
        $string = substr($string, 0, 8) . ' ' . substr($string, 8);
        $string = substr($string, 0, 11) . ' ' . substr($string, 11);
        $string = " 01 04 " . $string;
        return $string;
    }

    function converToString($data)
    {
        $string = ' ';
        if ($data <= '15') {
            $data = dechex($data);
            $string = $string . "0" . $data;
        } else {
            $data = dechex($data);
            $string = $string . $data;
        }
        return $string;
    }
}
