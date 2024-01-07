<title>Khu điều khiển 2</title>
@extends ('base.dashboard')
@section('content')
    <!-- Content Header (Page header) -->
<section class="content-header">
  <div class="container-fluid">
    <div class="row mb-2">
      <div class="col-sm-6">
        <h1 style="font-size: 25px;">
          HỆ THỐNG TƯỚI TIÊU
        </h1>
      </div>
    </div>
  </div><!-- /.container-fluid -->
</section>

    <!-- Main content -->

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


<script>
  $(function() {
    var status = false;
    var Toast = Swal.mixin({
      toast: true,
      position: 'top-end',
      showConfirmButton: false,
      timer: 3000
    });
    $('.swalDefaultSuccess').click(function() {
      Toast.fire({
        icon: 'success',
        title: 'Thiết lập thành công.'  
      })
      if(status == false){
        document.getElementById('bulb').src = "https://media.geeksforgeeks.org/wp-content/uploads/ONbulb.jpg";
        status = true;
      }
      else {
        document.getElementById('bulb').src = "https://media.geeksforgeeks.org/wp-content/uploads/OFFbulb.jpg";
        status = false;
      }
    });
  });

</script>
</body>
</html>
@endsection