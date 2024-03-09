<?php
$path = "./albums";
$imgType = array('png','jpg','mp4');
$show_begin = 0;
$step = 20;

$handle = opendir($path);
while(($file = readdir($handle)) != false){
    list($filename , $kzm) = explode('.', $file);
    if (in_array($kzm, $imgType)) {
        if (!is_dir('/'.$file)){ 
            $filelist[] = $file;
        }
    }
}
if(sizeof($filelist) > 0)
    $total_page = ceil(sizeof($filelist)/$step);
else $total_page = 0;

if(isset($_GET["p"])) $page = $_GET["p"];
else $page = 1;


function getFolderSize($dir) {
    $size = 0;
    if (is_dir($dir)){
        $dh = opendir($dir);
        while(false !== ($file = @readdir($dh))){
            if($file !='.' and $file !='..'){
                $path = $dir.'/'.$file;
                if(is_dir($path)){
                    $size += dir_size($path);
                }else if(is_file($path)){
                    $size += filesize($path);
                }
            }
        }
        closedir($dh);
    }else{
        echo  "路径不是文件夹路径";
    }
    return $size;
}

$kb = 1024;
$mb = 1024 * 1024;
$gb = 1024 * 1024 * 1024;
$limitSize = "64GB";

$size = getFolderSize($path);
$r = "B";

$pre = $size / ($gb*$limitSize);
$pre = round($pre, 2, PHP_ROUND_HALF_UP);

if($size > $kb && $size < $mb){
    $size = $size / $kb;
    $size = round($size, 2, PHP_ROUND_HALF_UP);
    $r = "KB";
}else if($size > $mb && $size < $gb){
    $size = $size / $mb;
    $size = round($size, 2, PHP_ROUND_HALF_UP);
    $r = "MB";
}else{
    $size = $size / $gb;
    $size = round($size, 2, PHP_ROUND_HALF_UP);
    $r = "GB";
}
?>

<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8" />
    <title>Albums</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
    <link rel="stylesheet" href="mdui/css/mdui.min.css">
    <?php if($page<$total_page) echo '<link rel="preload" href="/?p=' .$page+1 . '" as="style" />'; ?>
    <script>var total = <?php echo $total_page;?>; var pagenow = <?php echo $page;?>;</script>
    <script src="mdui/js/index.js"></script>
    <script src="mdui/js/mdui.min.js"></script>
</head>
<body class="mdui-bottom-nav-fixed">
    <div class="mdui-appbar mdui-color-grey-700">
        <div class="mdui-toolbar mdui-color-theme">
            <a href="https://blog.minloha.cn" class="mdui-typo-title">Micamera</a>
            <a href="javascript:new mdui.Dialog('#pre_pre').open();" class="mdui-typo-title"><?php echo $size . $r; ?>/<?php echo $limitSize; ?></a>
        </div>
    </div>
    <div class="mdui-container-fluid">
        <?php
            if(sizeof($filelist) != 0)
                for($i = ($page-1)*$step; $i < $page*$step; $i++){
                    if($i < sizeof($filelist)){
                        $f = $filelist[$i];
                        list($filename , $type_file) = explode('.', $f);
                        echo '<div class="mdui-card">';
                        echo '<div class="mdui-card-media">';
                        if($type_file == "jpg")
                            echo '<a href="./albums/'.$f.'"><img src="./albums/'.$f.'" style="width=100%;"/></a>';
                        else
                            echo '<a href="./albums/'.$f.'"><video><source src="./albums/'.$f.'" type="video/mp4"></video></a>';
                        echo '<div class="mdui-card-media-covered">';
                        echo '<div class="mdui-card-primary">';
                        echo '<div class="mdui-card-primary-title">'.$f.'</div>';
                        echo '</div>';
                        echo '</div>';
                        echo '</div>';
                        echo '<div class="mdui-card-actions">';
                        echo '<button class="mdui-btn mdui-ripple" onclick="remove(\'' . $f . '\')">删除</button>';
                        echo '<a style="text-decoration: none;color: black;" href="./albums/'.$f.'" download="/albums/' . $f . '"><button class="mdui-btn mdui-ripple" >保存</button></a>';
                        echo '</div>';
                        echo '</div>';
                    }
                }
            else{
                echo '<div class="mdui-dialog" id="dialog">';
                echo '<div class="mdui-dialog-title">Micamera提示</div>';
                echo '<div class="mdui-dialog-content">相册是空的,没有什么可以展示的呢~</div>';
                echo '<div class="mdui-dialog-actions">';
                echo '<button class="mdui-btn mdui-ripple">好吧</button>';
                echo '</div>';
                echo '</div>';
                echo "<script>new mdui.Dialog('#dialog').open()</script>";
            }
        ?>
        <?php
            if($pre > 0.9){
                echo '<div class="mdui-dialog" id="dialog">';
                echo '<div class="mdui-dialog-title">Micamera警告</div>';
                echo '<div class="mdui-dialog-content">内存空间不足,请删除照片!</div>';
                echo '<div class="mdui-dialog-actions">';
                echo '<button class="mdui-btn mdui-ripple">收到</button>';
                echo '</div>';
                echo '</div>';
                echo "<script>new mdui.Dialog('#dialog').open();</script>";
            }
        ?>
    </div>
    <div class="mdui-bottom-nav mdui-bottom-nav-text-auto mdui-color-grey-700">
        <a onclick="sub()" class="mdui-ripple mdui-ripple-white mdui-bottom-nav-active">
            <i class="mdui-icon material-icons">keyboard_arrow_left</i>
            <label>上一页</label>
        </a>
        <p>第<?php echo $page; ?> / <?php echo $total_page;?>页</p>
        <a onclick="add()" class="mdui-ripple mdui-ripple-white mdui-bottom-nav-active">
            <i class="mdui-icon material-icons">keyboard_arrow_right</i>
            <label>下一页</label>
        </a>
    </div>
    <div class="mdui-dialog" id="pre_pre">
        <div class="mdui-dialog-title">内存消耗情况:</div>
        
        <div class="mdui-p-a-2">
            <p>使用空间:<?php echo $size . $r; ?></p>
            <p>使用占比:</p>
        </div>
        <div class="mdui-progress">
            <div class="mdui-progress-determinate" style="width: <?php echo round($pre*100,0,PHP_ROUND_HALF_UP);?>%;"></div>
        </div>

        <div class="mdui-dialog-actions">
            <button class="mdui-btn mdui-ripple">我看懂了</button>
        </div>
    </div>
</body>
</html>