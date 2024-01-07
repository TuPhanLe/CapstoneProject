<div class="sidebar">
    <!-- Sidebar user panel (optional) -->
    <div class="user-panel mt-3 pb-3 mb-3 d-flex">
        <div class="image">
            <img src="{{asset("/assets/dist/img/avatar.png")}}" class="img-circle elevation-2" alt="User Image">
        </div>
        <div class="info">
            <a href="#" class="d-block">Chủ nông trại</a>
        </div>
    </div>

    <!-- SidebarSearch Form -->
    <div class="form-inline">
        <div class="input-group" data-widget="sidebar-search">
            <input class="form-control form-control-sidebar" type="search" placeholder="Search" aria-label="Search">
            <div class="input-group-append">
                <button class="btn btn-sidebar">
                    <i class="fas fa-search fa-fw"></i>
                </button>
            </div>
        </div>
    </div>

    <!-- Sidebar Menu -->
    <nav class="mt-2">
        <ul class="nav nav-pills nav-sidebar flex-column" data-widget="treeview" role="menu" data-accordion="false">
            <li class="nav-item menu-open">
                <a href="{{url("/setting")}}" class="nav-link active">
                    <i class="nav-icon fas fa-tools"></i>
                    <p>
                        Thiết lập
                        <i class="right fas "></i>
                    </p>
                </a>
            <li class="nav-item menu-open">
                <a href="#" class="nav-link active">
                    <i class="nav-icon fas fa-database"></i>
                    <p>
                        Dữ liệu
                        <i class="right fas fa-angle-left"></i>
                    </p>
                </a>
                <ul class="nav nav-treeview">
                    <li class="nav-item">
                        <a href="{{url("/area-1")}}" class="nav-link {{ (app('request')->route()->uri() == "first_page") ? "active" : ""}} ">
                            <i class="far fa-circle nav-icon"></i>
                            <p>Khu vực 1</p>
                        </a>
                    </li>
                    <li class="nav-item">
                        <a href="{{url("/area-2")}}" class="nav-link {{ (app('request')->route()->uri() == "second_page") ? "active" : ""}} ">
                            <i class="far fa-circle nav-icon"></i>
                            <p>Khu vực 2</p>
                        </a>
                    </li>
                </ul>
            <li class="nav-item menu-open">
                <a href="#" class="nav-link active">
                    <i class="nav-icon fas fa-toolbox"></i>
                    <p>
                        Điều khiển
                        <i class="right fas fa-angle-left"></i>
                    </p>
                </a>
                <ul class="nav nav-treeview">
                    <li class="nav-item">
                        <a href="{{url("/control-1")}}" class="nav-link {{ (app('request')->route()->uri() == "control-1") ? "active" : ""}} ">
                            <i class="far fa-circle nav-icon"></i>
                            <p>Hệ thống chiếu sáng</p>
                        </a>
                    </li>
                    <li class="nav-item">
                        <a href="{{url("/control-2")}}" class="nav-link {{ (app('request')->route()->uri() == "control-2") ? "active" : ""}} ">
                            <i class="far fa-circle nav-icon"></i>
                            <p>Hệ thống tưới tiêu</p>
                        </a>
                    </li>
            </li>
        </ul>

    </nav>

    <!-- /.sidebar-menu -->
</div>