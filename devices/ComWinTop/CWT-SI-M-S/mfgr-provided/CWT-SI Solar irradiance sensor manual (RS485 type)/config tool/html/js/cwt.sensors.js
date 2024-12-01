//------------------------------------------------------------------
//传感器定义
//只需在此文件定义就可以使用
//------------------------------------------------------------------
var Sensors = [
   
    //---------------------------------------------------CWT-SI Solar irradiance sensor定义开始
    {
        name: "CWT-SI Solar irradiance sensor",//在下拉中显示的名称
        img: "SI.jpg",//显示传感器的图片(imges中)
        slaveAddr: "",//默认传感器地址
        Registers: [
            {
                regName: "1: Solar irradiance value",
                regAddr: 1,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 1
            },
            {
                regName: "2: calibration",
                regAddr: 83,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 1,
                write: true,
            },
            {
                regName: "3: Baud",
                regAddr:2002,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 1,
                write: true,
            },
        ],
        QueryAddrHex: "FF 03 07 D0 00 01 91 59",//查询地址的数据，如果注释掉则无读从机地址的按钮
        slaveRegAddr: 0x7D0,//从机的地址所在的寄存器
        displayBtnWriteAddr: 1,//值0/1,0表示无写从机地址的按钮;1表示有写从机地址的按钮
        Tips: "Brown: DC10-30V  Black: GND  Yellow: A+  Blue: B-",
    },
    //---------------------------------------------------CWT-SI Solar irradiance sensor定义结束
    
  
];