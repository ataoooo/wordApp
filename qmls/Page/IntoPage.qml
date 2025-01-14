﻿import QtQuick 2.0
import QtGraphicalEffects 1.0
import "../Component"
//登陆界面
Rectangle{
    id:mainRec
    property bool savePsw: false
    anchors.fill: parent
    color: "transparent"


    Image {
        id: loginHead
        source: "../../assets/mdpi/top_bg.png"
        width: parent.width
        fillMode: Image.PreserveAspectFit
        z:0
    }


    Rectangle{
        id:fieldRec
        width: dp(82)
        height: width * 0.8
        radius: width * 0.1
        anchors{
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: dp(35.9)
        }
        layer.enabled: true
        layer.effect: DropShadow{
            radius: 10
            samples: 19
            verticalOffset: 0.5
            color: "#999999"
            transparentBorder: true
        }
        z:1
        Column{
            id:col
            width: parent.width * 0.95
            height: width
            anchors{
                top: parent.top
                topMargin: dp(9.2)
                horizontalCenter: parent.horizontalCenter
            }
            spacing: dp(5.56)
            InputEdit{
                id:userId
                width: parent.width
                height: dp(8)
                icon: "../../assets/mdpi/ic_login_user.png"
                placeholderText: "请输入用户帐号"
                //只接受数字和字母
                validator: RegExpValidator{regExp: /^\w*$/}
                implicitHeight: height*1.5
                text: config.getConfigBool("pwd/remeberMM",false) ? config.getConfigString("pwd/uid","") : ""
            }
            InputEdit{
                id:userPsw
                width: parent.width
                height: dp(8)
                placeholderText: "请输入密码"
                validator: RegExpValidator{regExp: /^\w*$/}
                icon:"../../assets/mdpi/ic_login_psw.png"
                echoMode: TextInput.Password
                implicitHeight: height * 1.5
                text: config.getConfigBool("pwd/remeberMM",false) ? config.getConfigString("pwd/upwd","") : ""
            }
        }

        //下方的登录注册按钮
        Row{
            id:btns
            anchors{
                bottom: parent.bottom
                bottomMargin: dp(14)
                horizontalCenter: parent.horizontalCenter
            }
            spacing: parent.width * 0.1
            Rectangle{
                id:enterBtn
                width: col.width * 0.37
                height: dp(10)
                radius: 5
                color: allColor
                Text {
                    anchors.centerIn: parent
                    text: qsTr("登录")
                    color: "white"
                    font.pixelSize: parent.height * 0.5
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked:{
                        var checkres = checkPwd();
                        console.log("The res = ",checkres)
                        if( checkres )
                        {
                            root.enterMainView()
                            config.setConfigBool("pwd/remeberMM",check.checked)
                            config.setConfigString("pwd/upwd",userPsw.text)
                            config.setConfigString("pwd/uid",userId.text)

                            //检查配置项
                            var confsno = config.getConfigInt("word/sno")
                            console.log("config sno = ",confsno," and ",root.userSno)
                            if(confsno != root.userSno){
                                config.setConfigInt("word/sno",root.userSno)
                                config.setConfigInt("word/num",0)
                                config.setConfigInt("word/yes",0)
                            }
                        }
                        else
                            win.visible = true
                    }
                }
            }
            Rectangle{
                width: col.width * 0.37
                height: dp(10)
                radius: 5
                color: "white"
                border{
                    color: allColor
                    width: 2
                }
                Text {
                    anchors.centerIn: parent
                    text: qsTr("注册")
                    color: allColor
                    font.pixelSize: parent.height * 0.5
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        root.loginView();
                    }
                }
            }
        }

        //记住密码 & 忘记密码
        Row{
            anchors{
                top: btns.bottom
                topMargin: dp(5)
                horizontalCenter: parent.horizontalCenter
            }
            spacing: dp(30)
            //记住密码
            Rectangle{
                id: check;
                property bool checked: config.getConfigBool("pwd/remeberMM",false)
                height: forgetPsw.height;
                width: checkImg.width + checkTxt.width;
                color: "transparent";
                Image {
                    id: checkImg;
                    source: parent.checked ? "../../assets/mdpi/radio_on.png":
                                            "../../assets/mdpi/radio_off.png";
                    height: checkTxt.height;
                    fillMode: Image.PreserveAspectFit;
                    MouseArea {
                        anchors.fill: parent;
                        onClicked: {
                            check.checked = !check.checked;
                        }
                    }
                    Rectangle {
                        anchors.centerIn: parent;
                        width: 0;
                        height: width;
                        radius: width / 2;
                        color: check.checked? allColor: "grey";
                        PropertyAnimation {
                            id: checkAni;
                            target: parent;
                            property: "width";

                        }
                    }
                }
                Text {
                    id: checkTxt;
                    anchors.left: checkImg.right;
                    anchors.leftMargin: dp(1);
                    text: "记住密码";
                    color: allColor;
                    font{
                        pixelSize: dp(4);
                        bold: true;
                    }
                }
            }
            Text {
                id: forgetPsw;
                text: "手机号登录";
                color: allColor;
                font{
                    pixelSize: dp(4);
                    bold: true;
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        root.pushStack(15)
                    }
                }
            }
        }
    }

    // 联系客服 & 体验一下
    Row{
        anchors{
            bottom: parent.bottom
            bottomMargin: dp(14)
            horizontalCenter: parent.horizontalCenter
        }
        spacing: dp(42)
        Image {
            id: totry
            source: "../../assets/mdpi/ic_login_ty.png"
            width: dp(6)
            height: width
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    root.userSno = -1
                    root.userName = "游客"
                    root.tablename = "allWords"
                    root.enterMainView()
                }
            }
            Text {
                anchors{
                    top: parent.bottom
                    topMargin: dp(1.8)
                    horizontalCenter: parent.horizontalCenter
                }
                text:"体验一下"
                color:allColor
            }
        }


        Image {
            id: toKF
            source: "../../assets/mdpi/ic_login_kf.png"
            width: dp(6)
            height: width
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    //???wait
                }
            }
            Text {
                anchors{
                    top: parent.bottom
                    topMargin: dp(1.8)
                    horizontalCenter: parent.horizontalCenter
                }
                text: qsTr("联系客服")
                color: allColor
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    root.showMsgHint("该功能暂未开放")
                }
            }
        }
    }
    Connections{
        target: myDB
    }

    //检查登录密码
    function checkPwd()
    {
        if(userId.text == "" || userPsw == "") return false;
        var uId = userId.recieveText();
        var upwd = userPsw.recieveText()
        var pwd =  myDB.findPwd(userId.recieveText());
        root.userPhone = myDB.getPhone(uId);
        root.userType = myDB.findType(uId);
        root.userName = uId
        root.userPwd = pwd
        root.userSno = myDB.getUserSno(uId);
        root.tablename = "allWords" + root.userSno
        console.log("userId is :", uId," - ",upwd," - ",pwd,"-",tablename);
        //为了方便
//        if( uId === "" || upwd === "" )
//            return false;
        if( upwd === pwd )
            return true;
        return false;
    }

    //错误提示框
    LittleWindow{
        id:win
        icon:"../../assets/mdpi/error.png"
        rectext: "用户名不存在或密码错误！"
        onBtnClicked: win.visible = false
    }

}
