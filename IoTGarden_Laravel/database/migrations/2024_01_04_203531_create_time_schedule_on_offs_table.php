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
        Schema::create('time_schedule_on_offs', function (Blueprint $table) {
            $table->increments('NUMBER');
            $table->timestamps();
            $table->string('STATUS')->nullable();
            $table->string('NOTE')->nullable();
            $table->string('START')->nullable();
            $table->string('DURATION')->nullable();
            $table->string('DAY')->nullable();
        });
    }

    /**
     * Reverse the migrations.
     */
    public function down(): void
    {
        Schema::dropIfExists('time_schedule_on_offs');
    }
};
