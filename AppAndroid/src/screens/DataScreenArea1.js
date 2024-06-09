/* eslint-disable prettier/prettier */
/* eslint-disable react-hooks/exhaustive-deps */
import React, { useEffect, useState, useContext} from 'react';
import {
StyleSheet,
View,
Text,
Pressable,
TextInput,
Image,
} from 'react-native';
import GlobalStyle from '../utils/GlobalStyle';
import AsyncStorage from '@react-native-async-storage/async-storage';
import {MyContext} from '../App';
import Button from '../components/Button';
import MqttService from '../helpers/MqttService';

const initData = {
temp: 0,
humidity: 0,
light: 0,
soil: 0,
};
const initDate = {
date: '',
time: '',
};
const topicToGW = 'server/to/gateway'
export default function DataScreenArea1({navigation, route}) {
const [timeCollectData, setTimeCollectData] = useState(1);
const [updateDate, setUpdateDate] = useState(initDate);
const [dataEnvironment, setDataEnvironment] = useState(initData);
const dataContext = useContext(MyContext);
useEffect(() => {
if (dataContext.receiNodeSensor1) {
    getDataSensor();
    updateTime();
}
}, [dataContext.receiNodeSensor1]);

useEffect(() => {
    getTimePeriod();
    getDataSensor();
    getUpdateTime();
}
, []);


const updateTime = () => {
let newDate = new Date().toLocaleDateString();
let newTime = new Date().toLocaleTimeString();
setUpdateDate({date: newDate, time: newTime});
storeUpdateTime({date: newDate, time: newTime});
};
const getDataSensor = async () => {
try {
    let dataSensor = await AsyncStorage.getItem('dataSensor1');
    setDataEnvironment(JSON.parse(dataSensor));
    console.log('Parse data sensor 1 success',JSON.parse(dataSensor));
    dataContext.setReceiNodeSensor1(false);
} catch (e) {
    // error reading value
}
};

const storeUpdateTime = async value => {
    try {
        const valueJSON = JSON.stringify(value);
        await AsyncStorage.setItem('time-update-node1', valueJSON);
        console.log('set time update success', valueJSON);
    } catch (e) {
        console.log(e);
    }
    };
const getUpdateTime = async () => {
try {
    let timeUpdate = await AsyncStorage.getItem('time-update-node1');
    if (JSON.parse(timeUpdate) != null) {setUpdateDate(JSON.parse(timeUpdate));}
    console.log('parse time update success', JSON.parse(timeUpdate));
} catch (e) {
    // error reading value
}
};
const storeTimePeriod = async value => {
try {
    await AsyncStorage.setItem('time-period', value);
    console.log('set time collect success');
} catch (e) {
    console.log(e);
}
};
const getTimePeriod = () => {
try {
    AsyncStorage.getItem('time-period').then(value => {
    setTimeCollectData(value);
    });
} catch (error) {
    console.log(error);
}
};

const HandleSetTime = value => {
    setTimeCollectData(value);
};

const HandleAreaScreen2 = () => {
    navigation.navigate('DỮ LIỆU KHU VỰC 2');
    };
const onSave = value => {
    SendPeriodCommand(timeCollectData);
    storeTimePeriod(timeCollectData);
};  
function ConvertPeriodToCommand(value) {
    var valueInSec = value * 60;
    var valueInHex = valueInSec.toString(16).toUpperCase();
    var zeroString = '0'.repeat(8 - valueInHex.length);
    var newString = zeroString.concat(valueInHex);
    var pairs = newString.match(/.{1,2}/g);
    return ' 01 04 ' + pairs.join(' ') + ' ';
}
const SendPeriodCommand = value => {
    var command = ConvertPeriodToCommand(value);
    const service = new MqttService()
    const success = () => {
        console.log('Connected to server');
        console.log('topic: ', topicToGW);
        service.sendMessage(`${topicToGW}`, command)
    }
    service.connect(success)
}

return (
<View style={styles.body}>
    <Image
    source={require('../../assets/pic/garden_0.png')}
    style={styles.image}
    />
    <View style={styles.directPress}>
    <Pressable
        style={({pressed}) => [
        styles.pressedStyleInScreen,
        pressed && {opacity: 0.6, backgroundColor: '#ddd'},
        ]}>
        <Text style={[styles.textButton, GlobalStyle.CustomFont]}>KHU VỰC 1</Text>
    </Pressable>
    <Pressable
        onPress={HandleAreaScreen2}
        style={({pressed}) => [
        styles.pressedStyle,
        pressed && {opacity: 0.6, backgroundColor: '#ddd'},
        ]}>
        <Text style={[styles.text, GlobalStyle.CustomFont]}>KHU VỰC 2</Text>
    </Pressable>
    </View>
    <View style={styles.blockInput}>
    <Text style={styles.text}>
        THỜI GIAN THU THẬP DỮ LIỆU ĐỊNH KÌ: {timeCollectData} PHÚT
    </Text>
    <TextInput
        placeholder="Xin mời nhập thời gian thu thập (phút)"
        style={styles.input}
        keyboardType="numeric"
        onChangeText={value => {
        HandleSetTime(value);
        }}
    />
    <View style={styles.buttonContainer}>
        <Button fill={true} onPress={onSave} title={'Cài đặt'} />
    </View>
    </View>
    <Text style={styles.text}>
    Thời gian {updateDate.time} {updateDate.date}
    </Text>
    <View style={styles.blockData}>
    <View style={styles.block}>
    <View style={styles.dataSensor}>
        <Text style={styles.text}>NHIỆT ĐỘ</Text>
        <Text style={styles.text}>{dataEnvironment.temp} °C</Text>
        <Image
        source={require('../../assets/pic/temperature.png')}
        style={styles.imageInBlock}
        />
    </View>
    <View />
    </View>

    <View style={styles.block}>
    <View style={styles.dataSensor}>
        <Text style={styles.text}>ĐỘ ẨM</Text>
        <Text style={styles.text}>{dataEnvironment.humidity} %</Text>
        <Image
        source={require('../../assets/pic/humidity.png')}
        style={styles.imageInBlock}
        />
        </View>
    </View>
    <View />
    </View>
    <View style={styles.blockData}>
    <View style={styles.block}>
    <View style={styles.dataSensor}>
        <Text style={styles.text}>CƯỜNG ĐỘ ÁNH SÁNG</Text>
        <Text style={styles.text}>{dataEnvironment.light} LUX</Text>
        <Image
        source={require('../../assets/pic/light.png')}
        style={styles.imageInBlock}
        />
        <View />
    </View>
    <View />
    </View>
    <View style={styles.block}>
        <View style={styles.dataSensor}>
        <Text style={styles.text}>ĐỘ ẨM ĐẤT</Text>
        <Text style={styles.text}>{dataEnvironment.soil} %</Text>
        <Image
        source={require('../../assets/pic/soil.png')}
        style={styles.imageInBlock}
        />
        </View>
        <View />
    </View>
    </View>
</View>
);
}

const styles = StyleSheet.create({
body: {
flex: 1,
flexDirection: 'column',
alignItems: 'center',
backgroundColor: '#ffffff',
},
image: {
borderRadius: 20,
margin: 10,
width: 800,
height: 280,
resizeMode: 'stretch',
},
imageBlock: {
width: 50,
height: 50,
resizeMode: 'contain',
},
text: {
fontSize: 14,
fontWeight: 'bold',
margin: 10,
},

textButton: {
    fontSize: 14,
    fontWeight: 'bold',
    margin: 10,
    color:'#fff'
    },
block: {
flex: 1,
alignItems: 'flex-start',
padding: 10,
borderRadius: 10,
borderWidth: 1,
borderColor: '#ccc',
margin: 10,
width: 190,
height: 100,
},
blockData: {
flexDirection: 'row',
alignItems: 'center',
},
blockInput: {
padding: 10,
borderRadius: 10,
borderWidth: 1,
borderColor: '#ccc',
margin: 10,
width: 400,
height: 180,
backgroundColor: '#ffffff',
},
input: {
height: 40,
margin: 12,
borderWidth: 0.5,
padding: 10,
},
pressedStyle: {
alignItems: 'center',
justifyContent: 'center',
width: 180,
height: 40,
margin: 10,
borderRadius: 10,
borderWidth: 1,
borderColor: '#ccc',
backgroundColor: '#ffffff',
},
pressedStyleInScreen: {
    alignItems: 'center',
    justifyContent: 'center',
    width: 180,
    height: 40,
    margin: 10,
    borderRadius: 10,
    borderWidth: 1,
    borderColor: '#ccc',
    backgroundColor: '#008000',
    },
directPress: {
flexDirection: 'row',
alignItems: 'center',
justifyContent: 'center',
},
buttonContainer: {
flexDirection: 'row',
alignItems: 'center',
justifyContent: 'center',
    },
dataSensor: {
    position:'absolute',
},
imageInBlock: {
    position:'relative',
    bottom: 40,
    left: 140,
    width: 50,
    height: 50,
},
});
