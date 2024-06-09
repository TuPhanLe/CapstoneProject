/* eslint-disable prettier/prettier */
/* eslint-disable react-hooks/exhaustive-deps */
import React, {useEffect, useState, useContext} from 'react';
import {StyleSheet, View, Text, Pressable, Platform, PermissionsAndroid, Alert, Modal, ActivityIndicator, ToastAndroid } from 'react-native';

import AsyncStorage from '@react-native-async-storage/async-storage';
import GlobalStyle from '../utils/GlobalStyle';
import TextInput from '../components/TextInput';
import { BleError, BleManager, Characteristic } from 'react-native-ble-plx'
import { encode, decode } from 'base-64';
export const manager = new BleManager()

var cmd4 = 'asen3310101010'
export default function BLESettingScreen({navigation, route}) {
const [BTPermissionsGranted, setBTPermissionsGranted] = useState(false);
const [serviceUUID, setServiceUUID] = useState('')
const [uuid, setUuid] = useState('')
const [deviceId, setDeviceId] = useState('')
const [manager, setManager] = useState(null);
const [device, setDevice] = useState(null);
const [showSettingWifi, setShowSettingWifi] = useState(false);
const [nameWifi, setNameWifi] = useState('');
const [passWifi, setPassWifi] = useState('');
const [activityIndicator, setActivityIndicator] = useState(false);
useEffect(() => {
const bleManager = new BleManager();
setManager(bleManager);
requestBTPermissions();
return () => {
bleManager.destroy();   


};
}, []);
useEffect(() => {
if (device) {
// connectToDevice();
}
}, [device]);

const connectToDevice = async () => {
if (!device) return;
try {
await device.connect();
console.log('Connected to device:', device.name);
const deviceWithServices = await device.discoverAllServicesAndCharacteristics();
const services = await deviceWithServices.services();

for (let service of services) {
const characteristics = await service.characteristics();
    if (characteristics[0].isWritableWithResponse)
    {
        setServiceUUID(characteristics[0].serviceUUID);
        setUuid(characteristics[0].uuid);
        setDeviceId(characteristics[0].deviceId);
    }
    setShowSettingWifi(true);

}

} catch (error) {
console.error('Error connecting to device:', error);
}
};
async function requestBTPermissions() {
if (Platform.OS === 'ios') {
    setBTPermissionsGranted(true);
    return;
}
if (Platform.OS === 'android') {
    console.log('Request time');
    const result = await PermissionsAndroid.requestMultiple([
    PermissionsAndroid.PERMISSIONS.BLUETOOTH_SCAN,
    PermissionsAndroid.PERMISSIONS.BLUETOOTH_CONNECT,
    PermissionsAndroid.PERMISSIONS.ACCESS_FINE_LOCATION,
    ]);

    setBTPermissionsGranted(
    result['android.permission.BLUETOOTH_CONNECT'] === PermissionsAndroid.RESULTS.GRANTED &&
    result['android.permission.BLUETOOTH_SCAN'] === PermissionsAndroid.RESULTS.GRANTED && 
    result['android.permission.ACCESS_FINE_LOCATION'] === PermissionsAndroid.RESULTS.GRANTED
    );

    return;
}
};

const scanForDevices = () => {
manager.startDeviceScan(null, null, (error, scannedDevice) => {
if (error) {
Alert.alert('Error scanning for devices:', error.reason);
return;
}
if ( scannedDevice.name === null) {setActivityIndicator(true)}
if (scannedDevice.name === "ESP_GATTS_DEMO") {
    setActivityIndicator(false);
manager.stopDeviceScan();
console.log("ESP_GATTS_DEMO", "found")
setDevice(scannedDevice);
}
});
};

const ReceiveNotification = (error, characteristicData) => {
    if (error) {
        console.log(error);
    }
    else if(!characteristicData?.value) {
        console.log('No data Receive');
        ToastAndroid.show('Cài đặt wifi thất bại', ToastAndroid.SHORT, ToastAndroid.CENTER,);
        return
    }
    if(characteristicData.value) {Alert.alert('Cài đặt wifi thành công');
}
}

const sendData = (value) => {
if (device) {
    device.connect()
    .then(device => {
    return device.discoverAllServicesAndCharacteristics()
    }).then((device) => {

    device.writeCharacteristicWithoutResponseForService(serviceUUID, uuid, encode(value));
    device.monitorCharacteristicForService(serviceUUID, uuid, ReceiveNotification)
}) 
}
}
const sendWifi = () => {
if (device) {
    device.connect().then(device => {
    return device.discoverAllServicesAndCharacteristics()
}).then((device) => {
    // device.writeCharacteristicWithoutResponseForService('000000ff-0000-1000-8000-00805f9b34fb', '0000ff01-0000-1000-8000-00805f9b34fb', encode('asen33 10101010'))
})
}
}

const HandleDataSend = () => {
    if (passWifi.length < 8) {ToastAndroid.show('Trạng thái dữ liệu chưa chính xác', ToastAndroid.SHORT, ToastAndroid.CENTER,);}
    else {
        setShowSettingWifi(false)
        var cmd1 = String.fromCharCode(1);
        var cmd2 = String.fromCharCode(nameWifi.length);
        var cmd3 = String.fromCharCode(passWifi.length);
        var commandString = cmd1.concat(cmd2, cmd3)
        console.log(commandString.concat(nameWifi,passWifi));
        sendData(commandString.concat(nameWifi,passWifi));
    };
    }

return ( 

<View style={styles.body}>
<Modal
visible={showSettingWifi}
transparent
onRequestClose={() =>
    setShowSettingWifi(false)
}
animationType='slide'
hardwareAccelerated
>
    <View style={styles.centered_view}>
    <View style={styles.setting_modal}>
    <View style={styles.setting_title}>
        <Text style={styles.text}>KẾT NỐI THÀNH CÔNG</Text>
    </View>
    <View style={styles.setting_body}>
    <View style={styles.inputContainer}>
        <TextInput
    description={'TÊN WIFI'}
    style={styles.textInput}
    onChangeText={v => setNameWifi(v)}
    value={nameWifi}
    />
    <TextInput
    description={'MẬT KHẨU'}
    style={styles.textInput}
    onChangeText={v => setPassWifi(v)}
    value={passWifi}
    secure={true}
    />
    </View>

    </View>
    <Pressable
        onPress={HandleDataSend}
        style={styles.setting_button}
        android_ripple={{color:'#fff'}}
    >
        <Text style={styles.text}>OK</Text>
    </Pressable>
    </View>
</View>
</Modal>
<View style={styles.heading}>
    <Text style={styles.textHeading}> DANH SÁCH THIẾT BỊ</Text>
  </View>
  {activityIndicator && <View style={[styles.container, styles.horizontal]}>
    <ActivityIndicator size="large" color="#00ff00"  />

</View>}
{device && 
<View>
<Pressable
onPress={connectToDevice}
style={({pressed}) => [
styles.pressedStyleNode,
pressed && {opacity: 0.6, backgroundColor: '#ddd'},
]}>
<Text style={[styles.text, GlobalStyle.CustomFont]}>
{device.name}
</Text>
</Pressable>
</View>
} 
<View style={styles.block}>
<Pressable
    onPress={scanForDevices}
    style={({pressed}) => [
    styles.pressedStyle,
    pressed && {opacity: 0.6, backgroundColor: '#ddd'},
    ]}>
    <Text style={[styles.text, GlobalStyle.CustomFont]}>
    TÌM KIẾM
    </Text>
</Pressable>
</View>
</View>
);
}

const styles = StyleSheet.create({
body: {
flex: 1,
flexDirection: 'column',
},
block: {
flex: 1,
flexDirection: 'column',
alignItems: 'center',
justifyContent: 'flex-end',
bottom: 150,
},
pressedStyle: {
alignItems: 'center',
justifyContent: 'center',
width: 180,
height: 60,
padding: 10,
marginLeft: 20,
marginTop: 10,
borderRadius: 10,
borderWidth: 1,
borderColor: '#ccc',
backgroundColor: '#ffffff',
},
pressedStyleNode: {
alignItems: 'center',
justifyContent: 'center',
width: 380,
height: 60,
padding: 10,
marginLeft: 20,
marginTop: 10,
borderRadius: 10,
borderWidth: 1,
borderColor: '#ccc',
backgroundColor: '#ffffff',
},
text: {
color: '#333333',
margin: 10,
fontSize: 16,
fontWeight: 'bold',
textAlign: 'center',
},
textHeading: {
color: '#333333',
margin: 10,
fontSize: 20,
fontWeight: 'bold',
},
heading: {
margin: 20,
},
button: {
width: 150,
height: 50,
alignItems: 'center',
},
centered_view: {
flex: 1,
justifyContent: 'center',
alignItems: 'center',
backgroundColor: '#00000099'
},
setting_modal: {
width: 350,
height: 300,
backgroundColor: '#ffffff',
borderRadius: 20,
},
setting_title: {
height: 50,
justifyContent: 'center',
alignItems: 'center',
backgroundColor: '#d0d5dc',
borderTopRightRadius: 20,
borderTopLeftRadius: 20,
},
setting_body: {
height: 200,
justifyContent: 'center',
alignItems: 'flex-start',
left: 20,
},
setting_button:{
backgroundColor:'#ffffff',
borderBottomLeftRadius:20,
borderBottomRightRadius:20,
},
inputContainer:{
    width: '90%'
},
container: {
    flex: 1,
    justifyContent: 'center',
},
horizontal: {
    flexDirection: 'row',
    justifyContent: 'space-around',
    padding: 10,
},
});
