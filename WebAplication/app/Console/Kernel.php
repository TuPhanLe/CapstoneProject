<?php

namespace App\Console;

use Illuminate\Console\Scheduling\Schedule;
use Illuminate\Foundation\Console\Kernel as ConsoleKernel;
class Kernel extends ConsoleKernel
{
    /**
     * Define the application's command schedule.
     */
    protected function schedule(Schedule $schedule): void
    {
        // $schedule->command('inspire')->hourly();
        // $schedule->command('inspire')
        // ->everySecond();
        // $schedule->command('MqttBG')->everySecond();
        $schedule->command('MqttBG')->everySecond()->runInBackground();
        $schedule->command('TimeSchedule')->everyMinute()->runInBackground()->withoutOverlapping();


    }

    /**
     * Register the commands for the application.
     */
    protected function commands(): void
    {
        $this->load(__DIR__.'/Commands');

        require base_path('routes/console.php');
    }
}
