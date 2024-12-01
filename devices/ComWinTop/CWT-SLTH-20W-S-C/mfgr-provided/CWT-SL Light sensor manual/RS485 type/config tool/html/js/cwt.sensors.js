//------------------------------------------------------------------
//传感器定义
//只需在此文件定义就可以使用
//------------------------------------------------------------------
var Sensors = [
   
    //---------------------------------------------------SL Light sensor定义开始
    {
        name: "CWT-SL Light sensor",//在下拉中显示的名称
        img: "CWT-SL Light sensor.jpg",//显示传感器的图片(imges中)
        slaveAddr: "",//默认传感器地址
        Registers: [
            {
                regName: "1: Humidity",
                regAddr: 1,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 0.1
            },
            {
                regName: "2: Temperature",
                regAddr: 2,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 0.1
            },
            {
                regName: "3: Illumination（0-65535Lux）",
                regAddr:7,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 1
            },
            {
                regName: "4: Illumination（0-200000Lux)",
                regAddr: 7,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 100
            },
            {
                regName: "5: Baud",
                regAddr: 2002,
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
    //---------------------------------------------------SL Light sensor定义结束
    
];