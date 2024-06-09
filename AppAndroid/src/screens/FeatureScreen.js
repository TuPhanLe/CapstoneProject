/* eslint-disable prettier/prettier */
/* eslint-disable react-hooks/exhaustive-deps */
import React from 'react';
import {StyleSheet, View, Text, Pressable, Image} from 'react-native';
import GlobalStyle from '../utils/GlobalStyle';

export default function FeatureScreen({navigation, route}) {
const HandleDataScreen = () => {
navigation.navigate('DỮ LIỆU KHU VỰC 1');
};
const HandleControlScreen = () => {
    navigation.navigate('ĐIỀU KHIỂN');
    };
    
return (
<View style={styles.body}>
    <View>
    <Image
        source={require('../../assets/pic/fuction.jpg')}
        style={styles.image}
    />
    </View>
    <Pressable
    onPress={HandleDataScreen}
    style={({pressed}) => [
        styles.pressedStyle,
        pressed && {opacity: 0.6, backgroundColor: '#ddd'},
    ]}>
    <Text style={[styles.text, GlobalStyle.CustomFont]}>
        Dữ liệu
    </Text>
    </Pressable>
    <Pressable
    onPress={HandleControlScreen}
    margin={10}
    style={({pressed}) => [
        styles.pressedStyle,
        pressed && {opacity: 0.6, backgroundColor: '#ddd'},
    ]}>
    <Text style={[styles.text, GlobalStyle.CustomFont]}>
        Điều khiển 
    </Text>
    </Pressable>
    <Pressable
    margin={10}
    style={({pressed}) => [
        styles.pressedStyle,
        pressed && {opacity: 0.6, backgroundColor: '#ddd'},
    ]}>
    <Text style={[styles.text, GlobalStyle.CustomFont]}>
        Giám sát
    </Text>
    </Pressable>
</View>
);
}

const styles = StyleSheet.create({
body: {
alignItems: 'center',
},
text: {
fontSize: 25,
fontWeight: 'bold',
margin: 10,
},
image: {
width: 400,
height: 280,
resizeMode: 'contain',
},
block: {
alignItems: 'flex-start',
justifyContent: 'center',
width: 350,
height: 60,
padding: 10,
borderRadius: 10,
borderWidth: 1,
borderColor: '#ccc',
margin: 10,
backgroundColor: '#ffffff',
},
pressedStyle: {
alignItems: 'flex-start',
justifyContent: 'center',
width: 350,
height: 90,
padding: 10,
margin: 20,
borderRadius: 10,
borderWidth: 1,
borderColor: '#ccc',
backgroundColor: '#ffffff',
},

});
