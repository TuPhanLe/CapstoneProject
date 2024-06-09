/* eslint-disable prettier/prettier */
import React from 'react';
import {TextInput, View, StyleSheet, Text} from 'react-native';
import {colors} from '../utils/global';

export default function ({onChangeText, value, description, secure, min}) {
  return (
    <View style={styles.container}>
      <View style={styles.descriptionContainer}>
        <Text style={styles.descriptionText}>{description}</Text>
      </View>
      <TextInput
        style={styles.textInput}
        onChangeText={onChangeText}
        value={value}
        secureTextEntry={secure}
        minLength = {min}
      />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    marginTop: 5,
    marginBottom: 5,
  },
  textInput: {
    borderWidth: 1,
    borderRadius: 12,
    padding: 10,
    paddingTop: 5,
    paddingBottom: 5,
    borderColor: colors.GREY,
    fontWeight: 'bold',
  },
  descriptionContainer: {
    margin: 10,
    marginLeft: 0,
  },
  descriptionText: {
    fontWeight: 'bold',
    color: colors.BLUE,
  },
});
