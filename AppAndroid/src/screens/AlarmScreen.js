/* eslint-disable prettier/prettier */
/* eslint-disable react-hooks/exhaustive-deps */
import {Text, View} from 'react-native';
import {getAlarmState, getAllAlarms, disableAlarm, enableAlarm} from '../utils/alarm';
import React, {useEffect, useState} from 'react';
import GlobalStyle from '../utils/GlobalStyle';
import AlarmView from '../components/AlarmView';


export default function AlarmScreen({navigation}) {
const [alarms, setAlarms] = useState(null);
const [scheduler, setScheduler] = useState(null);

useEffect(() => {
navigation.addListener('focus', async () => {
    setAlarms(await getAllAlarms());
    setScheduler(setInterval(fetchState, 10000));
});
navigation.addListener('blur', async () => {
    clearInterval(scheduler);
});
fetchState();
}, []);

async function fetchState() {
const alarmUid = await getAlarmState();
if (alarmUid) {
    navigation.navigate('Ring', {alarmUid});
}
}

return (
<View style={GlobalStyle.container}>
    <View style={GlobalStyle.innerContainer}>
    {alarms && alarms.length === 0 && <Text>Không có lịch hẹn</Text>}
    {alarms &&
        alarms.map(a => (
        <AlarmView
            key={a.uid}
            uid={a.uid}
            onChange={async active => {
            if (active) {
                await enableAlarm(a.uid);
            } else {
                await disableAlarm(a.uid);
            }
            }}
            onPress={() => navigation.navigate('ĐIỀU CHỈNH HẸN GIỜ', {alarm: a})}
            title={a.title}
            hour={a.hour}
            minutes={a.minutes}
            days={a.days}
            isActive={a.active}
        />
        ))}
    </View>
</View>
);
}