import React, { Component } from 'react';
import serial from "./Serial"
import { Alert,Button,Container } from 'reactstrap';

class App extends Component {
    
    state = {
        buttonState: 0,
        connected: 0
    }

    send = (text) => {
        const textEncoder = new TextEncoder();
        const data = textEncoder.encode(text)
        this.port.send(data)
    }


    receive = (data) => {
        const textDecoder = new TextDecoder()
        const text = textDecoder.decode(data).trim()
        if (text === "TURNED:0"){
            console.log("turning off")
            this.setState({buttonState:0})
        } else if  (text === "TURNED:1"){
            this.setState({buttonState:1})
            console.log("turning on")
        } else{
            console.log(text);
        }
    }


    connect = () => {
        console.log('Connecting to ' + this.port.device_.productName + '...');
        this.port.connect().then(() => {
            console.log('Connected.');
            this.setState({connected:1})
            this.port.onReceive = (data) => this.receive(data)
            this.port.onReceiveError = error => {
                console.log('Receive error: ' + error);
                this.setState({connected:0})
            }
        }, error => {
                console.log('Connection error: ' + error);
                this.setState({connected:0})
        });
    };



    handleConnect = () => {
    
        serial.getPorts().then((ports) => {
            console.log("after serial.getPorts")
            if (ports.length === 0) {
                console.log('No ports found.');
                this.port = serial.requestPort()
                this.connect();
            } else {
                console.log('Found port');
                this.port = ports[0]
                this.connect();
            }
        })
    }

    toggleButton = () => {
        this.setState({
            buttonState: this.state.buttonState ? 0 : 1
        })
        if (this.port){
            this.send(this.state.buttonState ? "L" : "H")
        }
    }


    render() {
        const connectButton = this.state.connected 
        ? 
        <Alert color="success">
            Connected to device
        </Alert>
        : 
        <Alert color="secondary">
            Not connected <Button className="ml-5" onClick={this.handleConnect}>Connect</Button>
        </Alert>
        
        

        return (
            <Container className="App mt-5">
                {connectButton}      
                <h1 >{this.state.buttonState?"ON":"OFF"}</h1>
                <Button color="info" onClick={this.toggleButton}>Toggle</Button>
            </Container>
        );
    }
}

export default App;
