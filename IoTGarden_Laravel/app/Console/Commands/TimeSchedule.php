<?php

namespace App\Console\Commands;

use App\Models\TimeScheduleOnOff;
use Illuminate\Console\Command;
use PhpMqtt\Client\MqttClient;
use PhpMqtt\Client\ConnectionSettings;

class TimeSchedule extends Command
{
    /**
     * The name and signature of the console command.
     *
     * @var string
     */
    protected $signature = 'TimeSchedule';

    /**
     * The console command description.
     *
     * @var string
     */
    protected $description = 'Command description';

    /**
     * Execute the console command.
     */

    public function handle()
    {
        $isScheduled = false;
        $startRealtime = date('H:i');
        $dayRealtime = $this->convertDayOfWeek(date('D'));

        $startRealtimeFake = '00:41';

        $duration = 0;
        $start = 0;
        foreach (TimeScheduleOnOff::where('DAY', '!=', null)->cursor() as $TimeScheduleOnOff) {
            if ($TimeScheduleOnOff->START === $startRealtime) {
                if ((strpos($TimeScheduleOnOff->DAY, $dayRealtime) !== false) || ($TimeScheduleOnOff->DAY === 'Hằng ngày ')) {
                    $isScheduled = true;
                    $duration = $TimeScheduleOnOff->DURATION;
                    $start = $TimeScheduleOnOff->START;
                }
            }
        }
        echo($this->SendMessageToGW($isScheduled, $start, $duration));
    }

    public function SendMessageToGW($isScheduled, $start, $duration)
    {

        if ($isScheduled) {
            $client = new MqttClient('569ed0169886465b9cb99dd61e2a369c.s2.eu.hivemq.cloud', 8883, 'test-publisher', MqttClient::MQTT_3_1, null, null);
            $connectionSettings = (new ConnectionSettings)
                ->setUsername('client02')
                ->setPassword('123123Aa')
                ->setUseTls(true)
                ->setTlsSelfSignedAllowed(true)
                ->setTlsVerifyPeer(false);
            $client->connect($connectionSettings, true);
            $dataSend = $this->convertTimeToGw($start, $duration);
            $client->publish('server/to/gateway', $dataSend, MqttClient::QOS_AT_MOST_ONCE);
            return true;
        }
        return false;
    }

    public function convertDayOfWeek($dayOfWeekInE)
    {
        $string = '';
        switch ($dayOfWeekInE) {
            case 'Mon':
                $string = 'Thứ 2 ';
                break;
            case 'Tue':
                $string = 'Thứ 3 ';
                break;
            case 'Wed':
                $string = 'Thứ 4 ';
                break;
            case 'Thu':
                $string = 'Thứ 5 ';
                break;
            case 'Fri':
                $string = 'Thứ 6 ';
                break;
            case 'Sat':
                $string = 'Thứ 7 ';
                break;
            case 'Sun':
                $string = 'Chủ nhật ';
                break;
        }
        return $string;
    }

    public function convertTimeToGw($start, $duration)
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
            while ($minEnd > 60) {
                $hourEnd++;
                $minEnd = $minEnd - 60;
            }
        }
        $string = $string . ($this->converToString($hourStart));
        $string = $string . ($this->converToString($minuStart));
        $string = $string . ($this->converToString($hourEnd));
        $string = $string . ($this->converToString($minEnd));
        $string = " 03 06 00 01" . $string . " ";
        return $string;
    }

    public function converToString($data)
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
