<title>Thiết lập</title>
@extends ('base.dashboard')
@section('content')
<!-- Content Header (Page header) -->
<section class="content-header">
    <div class="container-fluid">
        <div class="row mb-2">
            <div class="col-sm-6">
                <h1 style="font-size: 25px;">
                </h1>
            </div>
        </div>
    </div><!-- /.container-fluid -->
</section>

<!-- Main content -->
<section class="content">
    <div class="row">
        <div class="col-md-6">
            <div class="card card-primary" style="height: 400px; text-align: center">
                <!-- /.card-header -->
                <div class="card-header">
                    <h3 class="card-title ">TRẠNG THÁI KẾT NỐI</h3>
                    <div class="card-tools">
                        <button type="button" class="btn btn-tool" data-card-widget="collapse" title="Collapse">
                            <i class="fas"></i>
                        </button>
                    </div>
                </div>
                <div class="card-body p-0">
                    <table class="table table-sm">
                        <thead>
                            <tr>
                                <th style="width: 50%; font-size: 20px">KHỐI</th>
                                <th style="width: 50%; font-size: 20px">TRẠNG THÁI</th>
                            </tr>
                        </thead>
                        <tbody>
                            <tr>
                                <td>GATEWAY</td>
                                <td>
                                    <span id="jv-gateway" class="badge bg-danger" style="padding: 10px">
                                        MẤT KẾT NỐI
                                    </span>
                                </td>
                            </tr>
                            <tr>
                                <td>NODE CONTROL</td>
                                <td>
                                    <span id="jv-control" class="badge bg-danger" style="padding: 10px">
                                        MẤT KẾT NỐI
                                    </span>
                                </td>
                            </tr>
                            <tr>
                                <td>NODE SENSOR 1</td>
                                <td>
                                    <span id="jv-sensor-1" class="badge bg-danger" style="padding: 10px">
                                        MẤT KẾT NỐI
                                    </span>
                                </td>
                            </tr>
                            <tr>
                                <td>NODE SENSOR 2</td>
                                <td>
                                    <span id="jv-sensor-2" class="badge bg-danger" style="padding: 10px">
                                        MẤT KẾT NỐI
                                    </span>
                                </td>
                            </tr>
                        </tbody>
                    </table>
                </div>
                <div class="row">
                    <div class="col-md-6" style="
                                                padding: 20px;
                                                top: -10px;
                                                right: -52px;
                                            ">
                        <input type="submit" value="Kiểm tra" class="btn btn-success float-right swalDefaultSuccess">
                    </div>
                </div>
                <!-- /.card-body -->
            </div>
            <!-- /.card-body -->

        </div>
        <!-- /.card -->
        <div class="col-md-6">
            <div class="card card-primary" style="height: 250px;">
                <div class="card-header">
                    <h3 class="card-title ">THỜI GIAN THU THẬP DỮ LIỆU ĐỊNH KỲ</h3>
                    <div class="card-tools">
                        <button type="button" class="btn btn-tool" data-card-widget="collapse" title="Collapse">
                            <i class="fas"></i>
                        </button>
                    </div>
                </div>
                <div class="card-body" style="display: block; padding: 5px">
                    <form action="">
                        <div class="form-group" style="padding-left: 20px">
                            <label for="input-duration" style="font-size: 20px;">Nhập chu kỳ lấy dữ liệu</label>
                            <input type="number" id="input-duration" maxlength="3" class="form-control " placeholder="phút" style="
                                            margin-top: 15px;
                                        ">
                        </div>
                    </form>
                </div>
                <div class="row">
                    <div class="col-md-6" style="padding: 20px;
                                                top: -10px;
                                                right: -52px;
                                                ">
                        <input type="submit" value="Thêm" class="btn btn-success float-right swalSchedule">
                    </div>
                </div>
            </div>
            <!-- /.card-body -->
            <!-- /.card -->
        </div>
    </div>
    </div>
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

        $('.swalSchedule').click(function() {
            var inputDuration = document.getElementById('input-duration').value;
            var action = 0;
            console.log(inputDuration);
            if (inputDuration != 0) {
                $.ajax({
                    headers: {
                        'X-CSRF-TOKEN': $('meta[name="csrf-token"]').attr('content')
                    },
                    url: 'http://127.0.0.1:8000/setting/action',
                    method: 'POST',
                    data: {
                        'ACTION': action,
                        'DURATION': inputDuration,
                    },
                    success: function() {
                        Toast.fire({
                            icon: 'success',
                            title: 'Thiết lập thời gian thành công.'
                        })
                    }
                });
            }
        });
        $('.swalDefaultSuccess').click(function() {
            var action = 1;
            console.log(action);
            $.ajax({
                headers: {
                    'X-CSRF-TOKEN': $('meta[name="csrf-token"]').attr('content')
                },
                url: 'http://127.0.0.1:8000/setting/action',
                method: 'POST',
                data: {
                    'ACTION': action,
                },
                success: function() {
                    Toast.fire({
                        icon: 'success',
                        title: 'Đã gửi gói tin kiểm tra hệ.'
                    })
                }
            });
        });
    });



    setTimeout(() => {
        window.Echo.channel('BroadcastConnection')
            .listen('.App\\Events\\BroadcastConnection', (e) => {
                console.log(e['gateway']);
                if (e['gateway'] == 1) {
                    document.getElementById("jv-gateway").innerHTML = 'ĐÃ KẾT NỐI';
                    document.getElementById("jv-gateway").classList.remove('bg-danger');
                    document.getElementById("jv-gateway").classList.add('bg-success');
                }
                if (e['control'] == 1) {
                    document.getElementById("jv-control").innerHTML = 'ĐÃ KẾT NỐI';
                    document.getElementById("jv-control").classList.remove('bg-danger');
                    document.getElementById("jv-control").classList.add('bg-success');
                }
                if (e['sensor_1'] == 1) {
                    document.getElementById("jv-sensor-1").innerHTML = 'ĐÃ KẾT NỐI';
                    document.getElementById("jv-sensor-1").classList.remove('bg-danger');
                    document.getElementById("jv-sensor-1").classList.add('bg-success');
                }
                if (e['sensor_2'] == 1) {
                    document.getElementById("jv-sensor-2").innerHTML = 'ĐÃ KẾT NỐI';
                    document.getElementById("jv-sensor-2").classList.remove('bg-danger');
                    document.getElementById("jv-sensor-2").classList.add('bg-success');
                }

            })
    }, 1000);
</script>
</body>

</html>
@endsection