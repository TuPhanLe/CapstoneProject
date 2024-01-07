<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Support\Facades\DB;
use Illuminate\Database\Eloquent\Model;

class TimeScheduleOnOff extends Model
{
    use HasFactory;
    protected $fillable = [
        'STATUS', 
        'NOTE', 
        'START', 
        'END', 
        'DURATION',
        'DAY'
    ];
    public function addDataToDB($data) {
        DB::insert('INSERT INTO time_schedule_on_offs (STATUS, NOTE, START, END, DURATION) values (?, ?, ?, ?, ?)', $data);
    }
    public function showDataFromDB($flag){
        if($flag === 1) {
            $dataFromDB = DB::select('SELECT * FROM `time_schedule_on_offs` WHERE `node` = ? order BY  `time` DESC LIMIT 1', ['1']);
        }
        if($flag === 2) {
            $dataFromDB = DB::select('SELECT * FROM `time_schedule_on_offs` WHERE `node` = ? order BY  `time` DESC LIMIT 1', ['2']);
        }
            return $dataFromDB;
    }
}
