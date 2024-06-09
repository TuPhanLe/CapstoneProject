/* eslint-disable prettier/prettier */
/* eslint-disable react-hooks/exhaustive-deps */
import AsyncStorage from '@react-native-async-storage/async-storage';
import init from 'react_native_mqtt';

const topic = 'test';
const mqtt_connection_uri =
  '569ed0169886465b9cb99dd61e2a369c.s2.eu.hivemq.cloud';
const userName = 'client03';
const password = '123123Aa';
const mqtt_port = 8884;
init({
  size: 10000,
  storageBackend: AsyncStorage,
  defaultExpires: 1000 * 3600 * 24,
  enableCache: true,
  sync: {},
});
class MqttService {
  client = new Paho.MQTT.Client(
    mqtt_connection_uri,
    mqtt_port,
    'clientId-' + parseInt(Math.random() * 100, 10),
  );
  constructor() {}
  connect(callBack) {
    this.client.connect({
      onSuccess: callBack,
      useSSL: true,
      timeout: 5,
      onFailure: this.onFailure,
      userName: userName,
      password: password,
    });
  }

  checkConnect = () => {
    console.log(this.client.isConnected());
  };

  onFailure = err => {
    console.log('Connect failed!');
    console.log(err);
  };
  onConnectionLost = responseObject => {
    while (responseObject.errorCode !== 0) {
      console.log('onConnectionLost:' + responseObject.errorMessage);
    }
  };
  subscribeTopic = topic => {
    this.client.subscribe(topic, {qos: 0});
  };
  sendMessage = (topic, payload) => {
    var message = new Paho.MQTT.Message(payload);
    message.destinationName = topic;
    this.client.send(message);
  };
  getCLient() {
    return this.client;
  }
}

export default MqttService;
