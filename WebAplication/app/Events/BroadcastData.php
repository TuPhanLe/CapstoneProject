<?php

namespace App\Events;

use Illuminate\Broadcasting\Channel;
use Illuminate\Broadcasting\InteractsWithSockets;
use Illuminate\Broadcasting\PresenceChannel;
use Illuminate\Broadcasting\PrivateChannel;
use Illuminate\Contracts\Broadcasting\ShouldBroadcast;
use Illuminate\Foundation\Events\Dispatchable;
use Illuminate\Queue\SerializesModels;

class BroadcastData implements ShouldBroadcast
{
    use Dispatchable, InteractsWithSockets, SerializesModels;

    /**
     * Create a new event instance.
     */
    public $node;
    public $temp;
    public $humi;
    public $soil;
    public $light;
    public $time;
    public function __construct($node, $temp, $humi, $soil, $light, $time)
    {
        //
        $this->node = $node;
        $this->temp = $temp;
        $this->humi = $humi;
        $this->soil = $soil;
        $this->light = $light;
        $this->time = $time;
    }

    /**
     * Get the channels the event should broadcast on.
     *
     * @return array<int, \Illuminate\Broadcasting\Channel>
     */
    public function broadcastOn(): array
    {
        return [
            new Channel('BroadcastDataNode1'),
        ];
    }
}
