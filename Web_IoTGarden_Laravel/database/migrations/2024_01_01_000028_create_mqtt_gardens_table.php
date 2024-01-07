<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

return new class extends Migration
{
    /**
     * Run the migrations.
     */
    public function up(): void
    {
        Schema::create('mqtt_gardens', function (Blueprint $table) {
            $table->string('cmd');
            $table->string('length');
            $table->string('node');
            $table->string('temp');
            $table->string('humi');
            $table->string('soil');
            $table->string('light');
            $table->string('time');
        });
    }

    /**
     * Reverse the migrations.
     */
    public function down(): void
    {
        Schema::dropIfExists('mqtt_gardens');
    }
};
