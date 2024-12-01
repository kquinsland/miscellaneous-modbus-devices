//------------------------------------------------------------------
//传感器定义
//只需在此文件定义就可以使用
//------------------------------------------------------------------
var Sensors = [
   
    //---------------------------------------------------RS485 Small Weather Station定义开始
    {
        name: "RS485 Small Weather Station",//在下拉中显示的名称
        img: "1.jpg",//显示传感器的图片(imges中)
        slaveAddr: "",//默认传感器地址
        Registers: [
            {
                regName: "1: Wind speed",
                regAddr: 501,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 0.01
            },
            {
                regName: "2: Wind direction",
                regAddr: 502,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 1
            },
            {
                regName: "3: Wind direction(0-7)",
                regAddr: 503,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 1
            },
            {
                regName: "4: Wind direction (0-360)°",
                regAddr: 504,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 1
            },
            {
                regName: "5: Humidity",
                regAddr: 505,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 0.1
            },
            {
                regName: "6: Temperature",
                regAddr: 506,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 0.1
            },
            {
                regName: "7: Noise",
                regAddr: 507,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 0.1
            },
            {
                regName: "8: PM2.5",
                regAddr: 508,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 1
            },
            {
                regName: "9: PM10",
                regAddr: 509,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 1
            },
            {
                regName: "10: Atmospheric pressure",
                regAddr: 510,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 0.1
            },
            {
                regName: "11: Illuminance(0-200000 Lux) high 16 byte",
                regAddr: 511,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 1
            },
            {
                regName: "12: Illuminance(0-200000 Lux) low 16 byte",
                regAddr: 512,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 1
            },
            {
                regName: "13: Illuminance(0-200000 Lux)",
                regAddr: 513,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 1
            },
            {
                regName: "14: Rainfall",
                regAddr: 514,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 0.1
            },
            {
                regName: "15: Baud",
                regAddr: 2002,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 1,
                write: true,
            },
            {
                regName: "16: reset rainfall(write 90)",
                regAddr: 24579,
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
    //---------------------------------------------------THC-S定义结束
     //---------------------------------------------------THCPH-S定义开始
    
    //---------------------------------------------------Customize定义结束
];