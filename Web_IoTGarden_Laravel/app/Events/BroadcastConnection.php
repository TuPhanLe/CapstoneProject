<?php

namespace App\Events;

use Illuminate\Broadcasting\Channel;
use Illuminate\Broadcasting\InteractsWithSockets;
use Illuminate\Broadcasting\PresenceChannel;
use Illuminate\Broadcasting\PrivateChannel;
use Illuminate\Contracts\Broadcasting\ShouldBroadcast;
use Illuminate\Foundation\Events\Dispatchable;
use Illuminate\Queue\SerializesModels;

class BroadcastConnection implements ShouldBroadcast
{
    use Dispatchable, InteractsWithSockets, SerializesModels;

    /**
     * Create a new event instance.
     */
    public $control;
    public $gateway;
    public $sensor_1;
    public $sensor_2;
    public function __construct($control, $gateway, $sensor_1, $sensor_2)
    {
        $this->control = $control; 
        $this->gateway = $gateway; 
        $this->sensor_1 = $sensor_1; 
        $this->sensor_2 = $sensor_2; 
    }

    /**
     * Get the channels the event should broadcast on.
     *
     * @return array<int, \Illuminate\Broadcasting\Channel>
     */
    public function broadcastOn(): array
    {
        return [
            new Channel('BroadcastConnection'),
        ];
    }
}
