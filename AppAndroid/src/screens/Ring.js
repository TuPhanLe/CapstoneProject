/* eslint-disable prettier/prettier */
/* eslint-disable react-hooks/exhaustive-deps */
import React, {useEffect, useState} from 'react';
import {StyleSheet, Text, View} from 'react-native';
import {getAlarm, snoozeAlarm, stopAlarm} from '../utils/alarm';
import Button from '../components/Button';
import {colors, globalStyles} from '../utils/global';
import MqttService from '../helpers/MqttService';

const topicToGW = 'server/to/gateway'

export default function ({route, navigation}) {
  const [alarm, setAlarm] = useState(null);

  useEffect(() => {
    const alarmUid = route.params.alarmUid;
    (async function () {
      const myAlarm = await getAlarm(alarmUid);
      console.log(myAlarm);
      setAlarm(myAlarm);
      SendAlarmCommand(myAlarm.hour, myAlarm.minutes, myAlarm.snoozeInterval);
    })();
  }, []);

  function ConverAlarmToCommand(hourStart, minuteStart, duration) {
    let hourEnd = Math.floor(hourStart + (duration / 60));
    let minuteEnd = minuteStart + (duration % 60);
    if (minuteEnd >= 60) {
        hourEnd++;
        minuteEnd -= 60;
    }
    const hexHourStart = hourStart.toString(16).padStart(2, '0').toUpperCase();
    const hexMinuteStart = minuteStart.toString(16).padStart(2, '0').toUpperCase();
    const hexHourEnd = hourEnd.toString(16).padStart(2, '0').toUpperCase();
    const hexMinuteEnd = minuteEnd.toString(16).padStart(2, '0').toUpperCase();

    return ` 03 06 00 01 ${hexHourStart} ${hexMinuteStart} ${hexHourEnd} ${hexMinuteEnd} `;
}
const SendAlarmCommand = (time, hour, duration) => {
    var command = ConverAlarmToCommand(time, hour, duration);
    const service = new MqttService()
    const success = () => {
        console.log('Connected to server');
        console.log('topic: ', topicToGW);
        service.sendMessage(`${topicToGW}`, command)
    }
    service.connect(success)
}

  if (!alarm) {
    return <View />;
  }

  return (
    <View style={globalStyles.container}>
      <View style={[globalStyles.innerContainer, styles.container]}>
        <View style={styles.textContainer}>
          <Text style={styles.clockText}>
            {alarm.getTimeString().hour} : {alarm.getTimeString().minutes}
          </Text>
          <Text style={styles.title}>{alarm.title}</Text>
        </View>
        <View style={styles.buttonContainer}>
          <Button
            title={'Dừng lại'}
            onPress={async () => {
              await stopAlarm();
              navigation.goBack();
            }}
          />
        </View>
      </View>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    justifyContent: 'space-around',
    alignItems: 'center',
  },
  clockText: {
    color: 'black',
    fontWeight: 'bold',
    fontSize: 50,
  },
  textContainer: {
    display: 'flex',
    alignItems: 'center',
  },
  buttonContainer: {
    display: 'flex',
    flexDirection: 'row',
    alignItems: 'center',
    justifyContent: 'space-around',
  },
  title: {
    fontWeight: 'bold',
    fontSize: 20,
    color: colors.GREY,
  },
});
