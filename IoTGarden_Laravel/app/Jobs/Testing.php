<?php

namespace App\Jobs;

use Illuminate\Bus\Queueable;
use Illuminate\Contracts\Queue\ShouldBeUnique;
use Illuminate\Contracts\Queue\ShouldQueue;
use Illuminate\Foundation\Bus\Dispatchable;
use Illuminate\Queue\InteractsWithQueue;
use Illuminate\Queue\SerializesModels;
use App\Models\MqttGarden;
use App\Events\BroadcastData;
use PhpMqtt\Client\MqttClient;
use PhpMqtt\Client\ConnectionSettings;
use App\Events\BroadcastDataNode2;
class Testing implements ShouldQueue
{
    use Dispatchable, InteractsWithQueue, Queueable, SerializesModels;

    /**
     * Create a new job instance.
     */
    public function __construct()
    {
        //
    }
    protected $mqtt;

    /**
     * Execute the job.
     */
    public function handle(): void
    {

        $client = new MqttClient('569ed0169886465b9cb99dd61e2a369c.s2.eu.hivemq.cloud', 8883, 'test-publisher', MqttClient::MQTT_3_1, null, null);

        // Create and configure the connection settings as required.
        $connectionSettings = (new ConnectionSettings)
        ->setUsername('client02')
        ->setPassword('123123Aa')
        ->setUseTls(true)
        ->setTlsSelfSignedAllowed(true) 
        ->setTlsVerifyPeer(false);           
        $client->connect($connectionSettings, true);
    
        $handler = function (MqttClient $client, string $topic, string $message, int $qualityOfService, bool $retained) {
            // echo sprintf('Received QoS level 1 message on topic [%s]: %s', $topic, $message);
            $this->addDataRaw($message);
            $client->interrupt();
        };


        $client->registerMessageReceivedEventHandler($handler);
        $client->subscribe('data_gateway/to/server');
        $client->loop(true);
    }
    private function addDataRaw($message) {
        $mqtt = new MqttGarden();
        $cmd = hexdec(substr($message,0,2));
        $length = hexdec(substr($message,2,2));
        $node = hexdec(substr($message,4,2));
        $temp = hexdec(substr($message,6,4))/100;
        $humi = hexdec(substr($message,10,4))/100;
        $soil = hexdec(substr($message,14,4))/100;
        $light = hexdec(substr($message,18,8))/100;
        $time = date('H:i:s - d/m/Y');
        $datatest = [
            $cmd,
            $length,
            $node,
            $temp,
            $humi,
            $soil,
            $light,
            date('H:i:s - d/m/Y')
        ];
        $mqtt->addDataToDB($datatest);
        if($node == '1') {
        event(new BroadcastData($node, $temp, $humi, $soil, $light, $time));
        echo ('Success broadcast to node 1');
        } 
        if($node == '2') {
        event(new BroadcastDataNode2($node, $temp, $humi, $soil, $light, $time));
        echo ('Success broadcast to node 2');
        } 
        echo ('Success');

    }
}
