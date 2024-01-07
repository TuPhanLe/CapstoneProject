<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use Illuminate\Support\Facades\DB;

class MqttGarden extends Model
{
    use HasFactory;
    protected $fillable = [
        'cmd',  
        'length',  
        'node',  
        'temp',  
        'humi',  
        'soil',  
        'light',
        'time'
    ];
    public function addDataToDB($data) {
        DB::insert('INSERT INTO mqtt_gardens (cmd, length, node, temp, humi, soil, light, time) values (?, ?, ?, ?, ?, ?, ?, ?)', $data);
    }
    public function showDataFromDB($flag){
        if($flag === 1) {
            $dataFromDB = DB::select('SELECT * FROM `mqtt_gardens` WHERE `node` = ? order BY  `time` ASC LIMIT 1', ['1']);
        }
        if($flag === 2) {
            $dataFromDB = DB::select('SELECT * FROM `mqtt_gardens` WHERE `node` = ? order BY  `time` ASC LIMIT 1', ['2']);
        }
            return $dataFromDB;
    }

}
