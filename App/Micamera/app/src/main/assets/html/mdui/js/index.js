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
    var httpRequest = new XMLHttpRequest();//��һ������������Ķ���
        httpRequest.open('GET', '/remove.php?f='+name, true);
        httpRequest.send();
        httpRequest.onreadystatechange = function () {
            if (httpRequest.readyState == 4 && httpRequest.status == 200) {
                var json = httpRequest.responseText;//��ȡ��json�ַ������������
                console.log(json);
                location.reload();
            }
        };
}