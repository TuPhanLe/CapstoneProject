<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use Illuminate\Support\Facades\DB;
class test extends Model
{
    use HasFactory;
    protected $fillable = [
        'dataraw'
    ];
    public function insertDataRaw($data) {
        DB::insert('INSERT INTO tests (dataraw) values (?)',$data);
    }
}
