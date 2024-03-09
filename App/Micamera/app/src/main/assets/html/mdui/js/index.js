var drop = function(page){
    var url = window.location.origin;
    window.location.href = url + "?p=" + page;
}

function add(){
    if(pagenow < total)
        pagenow++;
    drop(pagenow);
}

function sub(){
    if(pagenow > 1)
        pagenow--;
    drop(pagenow);
}


function remove(name){
    var httpRequest = new XMLHttpRequest();//第一步：建立所需的对象
        httpRequest.open('GET', '/remove.php?f='+name, true);
        httpRequest.send();
        httpRequest.onreadystatechange = function () {
            if (httpRequest.readyState == 4 && httpRequest.status == 200) {
                var json = httpRequest.responseText;//获取到json字符串，还需解析
                console.log(json);
                location.reload();
            }
        };
}