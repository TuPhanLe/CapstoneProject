/* eslint-disable prettier/prettier */
/* eslint-disable react-hooks/exhaustive-deps */

import {StyleSheet} from 'react-native';
export const colors = {
GREY: '#d0d5dc',
BLUE: '#1992fe',
};

export default StyleSheet.create({
CustomFont: {
fontFamily: 'Bevan-Regular',
},
inputText: {
width: 300,
borderWidth: 1,
borderColor: '#555',
color: '#333333',
borderRadius: 10,
backgroundColor: '#ffffff',
textAlign: 'center',
fontSize: 20,
marginTop: 130,
marginLeft: 50,
marginBottom: 10,
},
container: {
height: '100%',
width: '100%',
display: 'flex',
alignItems: 'center',
justifyContent: 'center',
backgroundColor: 'white',
},
innerContainer: {
width: '90%',
height: '90%',
display: 'flex',
alignItems: 'center',
},
});
