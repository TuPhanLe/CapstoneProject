<title>Khu điều khiển 2</title>
@extends ('base.dashboard')
@section('content')
<!-- Content Header (Page header) -->
<section class="content-header">
  <div class="container-fluid">
    <div class="row mb-2">
      <div class="col-sm-6">
        <h1 style="font-size: 25px;">
          HỆ THỐNG CHIẾU SÁNG
        </h1>
      </div>
    </div>
  </div><!-- /.container-fluid -->
</section>

<!-- Main content -->
<section class="content">
  <div class="container-fluid">
    <div class="row">
      <div class="col-lg-6">
        <div class="card card-success card-outline" style="height: 180px;">
          <div class="card-header">
            <h3 class="card-title">
              <i class="fas fa-wrench icon-padding"></i>
              ĐIỀU KHIỂN
            </h3>
          </div>
          <div class="card-body">
            <div>
              <h1 class="switch-char char-body">CÔNG TẮC</h1>
            </div>
            <div class="switch-bound">
              <span class="switchBM" style=" left: -100px;top: 7px;">
                <input type="checkbox" id="switcher" class="swalDefaultSuccess" action="" method="POST">
                <label for="switcher"></label>
              </span>
            </div>

          </div>
          <div>
          </div>
          <!-- /.card -->
        </div>
      </div>
      <!-- /.col -->
      <div class="col-lg-6">
        <div class="card card-success card-outline" style="height: 180px;">
          <div class="card-header">
            <h3 class="card-title">
              <i class="fas fa-lightbulb icon-padding"></i>
              TRẠNG THÁI
            </h3>
          </div>
          <div class="card-body">
            <h1 class="char-body">TRẠNG THÁI HỆ </h1>
            <div class="light-bulb">
              <img id="bulb" src="https://media.geeksforgeeks.org/wp-content/uploads/OFFbulb.jpg" alt="Light bulb">
            </div>
            <div>
              <p id="control-by"> </p>
            </div>
          </div>
          <div>
          </div>
          <!-- /.card -->
        </div>
      </div>
    </div>
    <!-- ./row -->
    <div class="row">
      <div class="col-md-6">
        <div class="card card card-success card-outline">
          <div class="card-header">
            <h3 class="card-title">
              <i class="fas fa-user-clock icon-padding"></i>
              HẸN GIỜ
            </h3>
          </div>
          <div class="card-body" style="display: block;">
            <form action="">

              <div class="form-group">
                <label for="input-note">Ghi chú</label>
                <input type="text" id="input-note" class="form-control ">
              </div>
              <div class="form-group">
                <label for="input-start">Thời gian bắt đầu</label>
                <input type="time" id="input-start" class="form-control ">
              </div>
              <div class="form-group">
                <label for="input-duration">Thời gian sử dụng</label>
                <input type="number" id="input-duration" maxlength="3" class="form-control " placeholder="phút">
              </div>
              <div>
            </form>
            <form action="/action_page.php">

              <label for="day-in-week">Chọn ngày trong tuần</label>
              <div id="day-in-week">
                <input type="checkbox" id="mon" name="day" value="2" class="input-checkbox">
                <label for="mon" class="input-dayinweek">Thứ 2</label>
                <input type="checkbox" id="tue" name="day" value="3" class="input-checkbox">
                <label for="tue" class="input-dayinweek">Thứ 3</label>
                <input type="checkbox" id="wed" name="day" value="4" class="input-checkbox">
                <label for="wed" class="input-dayinweek" style="padding-right: 65px">Thứ 4</label>
                <input type="checkbox" id="thur" name="day" value="5" class="input-checkbox">
                <label for="thur" class="input-dayinweek">Thứ 5</label>
                <br>
                <input type="checkbox" id="fri" name="day" value="6" class="input-checkbox">
                <label for="fri" class="input-dayinweek">Thứ 6</label>
                <input type="checkbox" id="sat" name="day" value="7" class="input-checkbox">
                <label for="sat" class="input-dayinweek">Thứ 7</label>
                <input type="checkbox" id="sun" name="day" value="8" class="input-checkbox">
                <label for="sun" class="input-dayinweek">Chủ Nhật</label>
                <input type="checkbox" id="daily" name="day" value="1" class="input-checkbox">
                <label for="daily" class="input-dayinweek">Hằng ngày</label>
              </div>
            </form>

          </div>
          <div class="row">
            <div class="col-md-6">
              <input type="submit" value="Thêm" class="btn btn-success float-right swalSchedule">
            </div>
          </div>
        </div>
        <!-- /.card-body -->

      </div>
      <!-- /.card -->
    </div>
    <div class="col-md-6">
      <!-- Default box -->
      <div class="card card card-success card-outline">
        <div class="card-header">
          <h3 class="card-title">
            <i class="fas fa-sticky-note icon-padding"></i>
            LỊCH HẸN GIỜ
          </h3>
        </div>
        <div class="card-body p-0">
          <table class="table table-striped projects">
            <thead>
              <tr>
                <th style="width: 1%; padding-left: 5px">
                  STT
                </th>
                <th style="width: 20%">
                  Ghi chú
                </th>
                <th style="width: 15%">
                  Bắt đầu
                </th>
                <th style="width: 15%">
                  Trong khoảng
                </th>
                <th style="width: 30%" class="text-center">
                  Ngày trong tuần
                </th>
                <th style="width: 10%">
                </th>
              </tr>
            </thead>
            <tbody>
              @php
              $count = 1;
              @endphp
              @foreach ($TimeSchedules as $key => $TimeSchedule )
              @if(!empty($TimeSchedule->START))
              <tr>
                <td style="padding-left: 8px;">
                  {{$count++}}
                </td>
                <td>
                  <a>
                    {{$TimeSchedule->NOTE}}
                  </a>
                  <br />
                </td>
                <td>
                  <ul class="list-inline">
                    {{$TimeSchedule->START}}
                  </ul>
                </td>
                <td>
                  <small>
                    {{$TimeSchedule->DURATION}}
                  </small>
                </td>
                <td>
                  <span>
                    {{$TimeSchedule->DAY}}
                  </span>
                </td>
                <td class="project-actions text-right">
                  <button class="btn btn-success btn-sm swalSchedule" href="#" id="delete-schedule" onclick="deleteSchedule('{{$TimeSchedule->NUMBER}}')" style="background-color: red; border-color: red">
                    <i class="fas fa-trash">
                    </i>
                    Xóa
                  </button>
                </td>
              </tr>
              @endif
              @endforeach
            </tbody>
          </table>
        </div>
        <!-- /.card-body -->
      </div>
      <!-- /.card -->
    </div>
  </div>
  </div><!-- /.container-fluid -->

</section>
<!-- /.content -->
</div>
<!-- /.content-wrapper -->

<!-- Control Sidebar -->
<aside class="control-sidebar control-sidebar-dark">
  <!-- Control sidebar content goes here -->
</aside>
<!-- /.control-sidebar -->
</div>
<!-- ./wrapper -->

@vite('resources/js/app.js')
<script>
  $(function() {
    var statusButtonLight = false;
    var statusButtonSchedule = false;
    var Toast = Swal.mixin({
      toast: true,
      position: 'top-end',
      showConfirmButton: false,
      timer: 3000
    });
    $('.swalDefaultSuccess').click(function() {
      if (statusButtonLight == false) {
        $.ajax({
          headers: {
            'X-CSRF-TOKEN': $('meta[name="csrf-token"]').attr('content')
          },
          url: 'http://127.0.0.1:8000/control-1/action',
          method: 'POST',
          data: {
            'STATUS': 'ON'
          },
          success: function() {
            Toast.fire({
              icon: 'success',
              title: 'Bật hệ.'
            })
          }
        });
        statusButtonLight = true;
      } else {
        $.ajax({
          headers: {
            'X-CSRF-TOKEN': $('meta[name="csrf-token"]').attr('content')
          },
          url: 'http://127.0.0.1:8000/control-1/action',
          method: 'POST',
          data: {
            'STATUS': 'OFF'
          },
          success: function() {
            Toast.fire({
              icon: 'success',
              title: 'Tắt hệ.'
            })
          }
        });
        statusButtonLight = false;
      }
    });
    $('.swalSchedule').click(function() {
      var inputNote = document.getElementById('input-note').value;
      var inputStart = document.getElementById('input-start').value;
      var inputDuration = document.getElementById('input-duration').value;
      var inputDay = checkDayInWeek();
      console.log(inputDay);
      $.ajax({
        headers: {
          'X-CSRF-TOKEN': $('meta[name="csrf-token"]').attr('content')
        },
        url: 'http://127.0.0.1:8000/control-1/action',
        method: 'POST',
        data: {
          'STATUS': 'SCHEDULE',
          'NOTE': inputNote,
          'START': inputStart,
          'DURATION': inputDuration,
          'DAY': inputDay,
        },
        success: function() {
          location.reload();
          Toast.fire({
            icon: 'success',
            title: 'Thên lịch thành công.'
          })
        }
      });
    });
  });

  function checkDayInWeek() {
    const inputDay = document.querySelectorAll('input[name="day"]');
    const day = Array.from(inputDay);
    const dayOn = [];
    for (const dayOfWeek of day) {
      if (dayOfWeek.checked) {
        dayOn.push(dayOfWeek.value);
      }
    }
    return dayOn;
  };

  function deleteSchedule(NUMBER) {
    console.log(NUMBER);
    $.ajax({
      headers: {
        'X-CSRF-TOKEN': $('meta[name="csrf-token"]').attr('content')
      },
      url: 'http://127.0.0.1:8000/control-1/delete',
      method: 'POST',
      data: {
        'NUMBER': NUMBER
      },
      success: function() {

        location.reload();
        Toast.fire({
          icon: 'success',
          title: 'Thên lịch thành công.'
        })
      }
    });
  };

  const image = document.getElementById("bulb");
  setTimeout(() => {
    window.Echo.channel('BroadcastState')
      .listen('.App\\Events\\BroadcastState', (e) => {
        console.log(e['state']);
        console.log(e['alarm']);

        if ((e['alarm'] == 1) && (e['state'] == 0)) {
          document.getElementById('control-by').innerHTML = 'Thao tác: Hẹn giờ';
          image.src = 'https://media.geeksforgeeks.org/wp-content/uploads/ONbulb.jpg'
        }
        if ((e['alarm'] == 0) && (e['state'] == 1)) {
          document.getElementById('control-by').innerHTML = 'Thao tác: điều khiển trực tiếp';
          image.src = 'https://media.geeksforgeeks.org/wp-content/uploads/ONbulb.jpg'
        }
        if ((e['state'] == 0) && (e['alarm'] == 0)) {
          console.log(e['alarm']);
          image.src = 'https://media.geeksforgeeks.org/wp-content/uploads/OFFbulb.jpg'
          document.getElementById('control-by').innerHTML = 'Đã tắt';
        }
      })
  }, 1000);
</script>
</body>

</html>
@endsection