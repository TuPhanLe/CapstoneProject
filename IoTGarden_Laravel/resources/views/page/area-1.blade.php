<title>Khu vực 1</title>
@extends ('base.dashboard')
@section('content')
  @if(!empty($data))
    @foreach($data as $key => $item)
<div class="content-header">
        <div class="container-fluid">
          <div class="row mb-2">
            <div class="col-sm-6">
              <h1 class="m-0" style="font-size: 25px;">VƯỜN RAU SÚP LƠ</h1>
            </div><!-- /.col -->
            <div class="col-sm-6">
              <ol class="breadcrumb float-sm-right">
                <!-- <li class="breadcrumb-item"><a href="#">Home</a></li> -->
                <li class="breadcrumb-item active">Dữ liệu cập nhật lần cuối tại {{$item->time}}</li>
                <li class="breadcrumb-item active " id="jv-time"> </li>
              </ol>
            </div>
          </div><!-- /.row -->
        </div><!-- /.container-fluid -->
  </div>
      <!-- /.content-header -->

    <!-- Main content -->
<div class="content">
      <div class="container-fluid">
        <div class="row">
          <div class="col-lg-6">
            <div class="card">
              <div class="card-header border-0">
                <div class="d-flex justify-content-between">
                  <h3 class="card-title">Nhiệt độ</h3>
                  <h3 class="card-title" id="jv-temp">{{$item->temp}}℃</h3>
                </div>
              </div>
              <div class="card-body">
                <div class="d-flex">
                  <p class="d-flex flex-column">
                    <span class="text-bold text-lg temp" id="jv-temp"></span>
                  </p>
                  <p class="ml-auto d-flex flex-column text-right">
                    <span class="text-success">
                    </span>
                  </p>
                </div>
                <!-- /.d-flex -->

                <div class="position-relative mb-4"><div class="chartjs-size-monitor"><div class="chartjs-size-monitor-expand"><div class=""></div></div><div class="chartjs-size-monitor-shrink"><div class=""></div></div></div>
                      <canvas id="js-Chart-Temp" style="height: 300px;"></canvas>  
                </div>
              </div>
            </div>
            <!-- /.card -->

            <div class="card">
              <div class="card-header border-0">
                <div class="d-flex justify-content-between">
                  <h3 class="card-title">Độ ẩm không khí</h3>
                  <h3 class="card-title" id="jv-humi">{{$item->humi}}%</h3>
                </div>
              </div>
              <div class="card-body">
                <div class="d-flex">
                  <p class="d-flex flex-column">
                    <span class="text-bold text-lg"  ></span>
                  </p>
                  <p class="ml-auto d-flex flex-column text-right">
                    <span class="text-success">
                    </span>
                  </p>
                </div>
                <!-- /.d-flex -->

                <div class="position-relative mb-4"><div class="chartjs-size-monitor"><div class="chartjs-size-monitor-expand"><div class=""></div></div><div class="chartjs-size-monitor-shrink"><div class=""></div></div></div>
                <canvas id="js-Chart-Humi" style="height: 300px"></canvas>  
                </div>
              </div>
            </div>
            <!-- /.card -->
          </div>
          <!-- /.col-md-6 -->
          <div class="col-lg-6">
          <div class="card">
              <div class="card-header border-0">
                <div class="d-flex justify-content-between">
                  <h3 class="card-title">Độ ẩm đất</h3>
                  <h3 class="card-title" id="jv-soil">{{$item->soil}}%</h3>
                </div>
              </div>
              <div class="card-body">
                <div class="d-flex">
                  <p class="d-flex flex-column">
                    <span class="text-bold text-lg" ></span>
                  </p>
                  <p class="ml-auto d-flex flex-column text-right">
                    <span class="text-success">
                    </span>
                  </p>
                </div>
                <!-- /.d-flex -->

                <div class="position-relative mb-4"><div class="chartjs-size-monitor"><div class="chartjs-size-monitor-expand"><div class=""></div></div><div class="chartjs-size-monitor-shrink"><div class=""></div></div></div>
                <canvas id="js-Chart-Soil" style="height: 300px"></canvas>  
                </div>
              </div>
            </div>
            <!-- /.card -->

        <div class="card">
              <div class="card-header border-0">
                <div class="d-flex justify-content-between">
                  <h3 class="card-title">Cường độ ánh sáng</h3>
                  <h3 class="card-title" id="jv-light">{{$item->light}} lux</h3>
                </div>
              </div>
              <div class="card-body">
                <div class="d-flex">
                  <p class="d-flex flex-column">
                    <span class="text-bold text-lg"></span>
                  </p>
                  <p class="ml-auto d-flex flex-column text-right">
                    <span class="text-success">
                    </span>
                  </p>
                </div>
                <!-- /.d-flex -->

                <div class="position-relative mb-4"><div class="chartjs-size-monitor"><div class="chartjs-size-monitor-expand"><div class=""></div></div><div class="chartjs-size-monitor-shrink"><div class=""></div></div></div>
                <canvas id="js-Chart-Light" style="height: 300px"></canvas>  
              </div>

              </div>
            </div>
          </div>
          <!-- /.col-md-6 -->
        </div>
        <!-- /.row -->
      </div>
      <!-- /.container-fluid -->
    </div>
    @endforeach
  @endif


  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  @vite('resources/js/app.js')
  <script>
  let chartTemp;
  let chartHumi;
  let chartSoil;
  let chartLight;
  let temp;
  let humi;
  let soil;
  let light
  let time;
  let test;
    const CTemp = document.getElementById('js-Chart-Temp');
    chartTemp = new Chart(CTemp, {
    type: 'line',
    data: {
    labels: time,
    datasets: 
    [{
        label: 'Nhiệt độ',
        data: temp,
        backgroundColor: ['rgb(255,99,132)'],
        borderWidth: 1
    }]
    },
    options: {
        responsive: true,
        maintainAspectRatio: false,
        scales: {
          x: {
            display: true,
            title: {
              display: true,
              text: 'Thời gian',
              color: '#911',
              font: {
                family: 'Time new roman',
                size: 12,
                weight: 'bold',
                lineHeight: 1.2,
              },
              // padding: {top: 20, left: -5, right: 0, bottom: 0}
        }
      },
        y: {
          display: true,
          title: {
            display: true,
            text: '℃',
            color: '#191',
            font: {
              family: 'Times',
              size: 12,
              style: 'normal',
              lineHeight: 1.2
            },
            // padding: {top: 30, left: -5, right: 0, bottom: 0}
          }
        }
        },

    }
    });
    const CHumi = document.getElementById('js-Chart-Humi');
    chartHumi = new Chart(CHumi, {
    type: 'line',
    data: {
    labels: time,
    datasets: 
    [{
        label: 'Độ ẩm', 
        data: temp,
        backgroundColor: ['rgb(99,99,132)'],
        borderWidth: 1
    }]
    },
    options: {
        responsive: true,
        maintainAspectRatio: false,
        scales: {
          x: {
            display: true,
            title: {
              display: true,
              text: 'Thời gian',
              color: '#911',
              font: {
                family: 'Time new roman',
                size: 12,
                weight: 'bold',
                lineHeight: 1.2,
              },
              // padding: {top: 20, left: -5, right: 0, bottom: 0}
        }
      },
        y: {
          display: true,
          title: {
            display: true,
            text: 'phần trăm',
            color: '#191',
            font: {
              family: 'Times',
              size: 12,
              style: 'normal',
              lineHeight: 1.2
            },
            // padding: {top: 30, left: -5, right: 0, bottom: 0}
          }
        }
        },

    }
    });

  const CSoil = document.getElementById('js-Chart-Soil');
    chartSoil = new Chart(CSoil, {
    type: 'line',
    data: {
    labels: time,
    datasets: 
    [{
        label: 'Độ ẩm đất', 
        data: temp,
        backgroundColor: ['rgb(99,255,132)'],
        borderWidth: 1
    }]
    },
    options: {
        responsive: true,
        maintainAspectRatio: false,
        scales: {
          x: {
            display: true,
            title: {
              display: true,
              text: 'Thời gian',
              color: '#911',
              font: {
                family: 'Time new roman',
                size: 12,
                weight: 'bold',
                lineHeight: 1.2,
              },
              // padding: {top: 20, left: -5, right: 0, bottom: 0}
        }
      },
        y: {
          display: true,
          title: {
            display: true,
            text: 'phần trăm',
            color: '#191',
            font: {
              family: 'Times',
              size: 12,
              style: 'normal',
              lineHeight: 1.2
            },
            // padding: {top: 30, left: -5, right: 0, bottom: 0}
          }
        }
        },

    }
    });
    const CLight = document.getElementById('js-Chart-Light');
    chartLight = new Chart(CLight, {
    type: 'line',
    data: {
    labels: time,
    datasets: 
    [{
        label: 'Cường độ ánh sáng', 
        data: temp,
        backgroundColor: ['blue'],
        borderWidth: 1
    }]
    },
    options: {
        responsive: true,
        maintainAspectRatio: false,
        scales: {
          x: {
            display: true,
            title: {
              display: true,
              text: 'Thời gian',
              color: '#911',
              font: {
                family: 'Time new roman',
                size: 10,
                weight: 'bold',
                lineHeight: 1.2,
              },
              // padding: {top: 20, left: -5, right: 0, bottom: 0}
        }
      },
        y: {
          display: true,
          title: {
            display: true,
            text: 'Lux',
            color: '#191',
            font: {
              family: 'Times',
              size: 10,
              style: 'normal',
              lineHeight: 1.2
            },
            // padding: {top: 30, left: -5, right: 0, bottom: 0}
          }
        }
        },

    }
    });

    setTimeout(() => {
        window.Echo.channel('BroadcastDataNode1')
            .listen('.App\\Events\\BroadcastData', (e) => {
            console.log(e);
            temp = e['temp'];
            humi = e['humi'];
            soil = e['soil'];
            light = e['light']
            time = e['time'];
            document.getElementById('jv-temp').innerHTML = temp;
            document.getElementById('jv-humi').innerHTML = humi;
            document.getElementById('jv-soil').innerHTML = soil;
            document.getElementById('jv-light').innerHTML = light;
            document.getElementById('jv-time').textContent = time;
            setDataChartTemp(e);
            setDataChartHumi(e);
            setDataChartSoil(e);
            setDataChartLight(e);
            })
    }, 1000);
    function setDataChartTemp(e) {
      chartTemp.data.labels.push(e.time);
      chartTemp.data.datasets[0].data.push(e.temp);
      chartTemp.update();
    }
    function setDataChartHumi(e) {
      chartHumi.data.labels.push(e.time);
      chartHumi.data.datasets[0].data.push(e.humi);
      chartHumi.update();
    }
    function setDataChartSoil(e) {
      chartSoil.data.labels.push(e.time);
      chartSoil.data.datasets[0].data.push(e.soil);
      chartSoil.update();
    }
    function setDataChartLight(e) {
      chartLight.data.labels.push(e.time);
      chartLight.data.datasets[0].data.push(e.light);
      // chart.data.datasets[0].data.shift();
      chartLight.update();
    }
  
  </script>
@endsection
