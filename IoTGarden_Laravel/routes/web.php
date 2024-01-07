<?php

use Illuminate\Support\Facades\Route;
use App\Http\Controllers\HomeController;
use App\Http\Controllers\BaseController;

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider and all of them will
| be assigned to the "web" middleware group. Make something great!
|
*/

Route::get('/', [HomeController::class,'index']);

Route::get('/area-1', [HomeController::class,'area_1']);

Route::get('/area-2', [HomeController::class,'area_2']);




Route::get('/control-2', [HomeController::class,'control_2']);

Route::prefix('setting')->name('setting.')->group(function() {
    Route::get('/',[HomeController::class, 'setting'])->name('show');
    Route::post('/action',[HomeController::class, 'setting_action']);

});


Route::prefix('control-1')->name('control-1.')->group(function() {
    Route::get('/',[HomeController::class, 'control_1'])->name('show');
    Route::post('/action',[HomeController::class, 'control_1_action']);
    Route::post('/delete',[HomeController::class, 'control_1_delete']);

});





