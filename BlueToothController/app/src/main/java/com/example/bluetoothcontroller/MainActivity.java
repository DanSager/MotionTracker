package com.example.bluetoothcontroller;

import androidx.appcompat.app.AppCompatActivity;

import app.akexorcist.bluetotohspp.library.BluetoothSPP;
import app.akexorcist.bluetotohspp.library.BluetoothSPP.AutoConnectionListener;
import app.akexorcist.bluetotohspp.library.BluetoothSPP.BluetoothStateListener;
import app.akexorcist.bluetotohspp.library.BluetoothSPP.BluetoothConnectionListener;
import app.akexorcist.bluetotohspp.library.BluetoothSPP.OnDataReceivedListener;
import app.akexorcist.bluetotohspp.library.BluetoothState;
import app.akexorcist.bluetotohspp.library.DeviceList;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    BluetoothSPP bt;
    final String INCREMENT = "i"; // i is short for increment
    final String DECREMENT = "d"; // d is short for decrement
    final String RESET = "r"; // r is short for reset

    Button connect;
    Button increment;
    Button decrement;
    Button reset;
    TextView count;
    TextView number;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        bt = new BluetoothSPP(this);

        connect = (Button) findViewById(R.id.connect);
        increment = (Button) findViewById(R.id.increment);
        decrement = (Button) findViewById(R.id.decrement);
        reset = (Button) findViewById(R.id.reset);
        count = (TextView) findViewById(R.id.count);
        number = (TextView) findViewById(R.id.number);

        if (!bt.isBluetoothAvailable()) {
            Toast.makeText(getApplicationContext(), "Bluetooth is not available", Toast.LENGTH_SHORT).show();
            finish();
        }

        bt.setBluetoothConnectionListener(new BluetoothSPP.BluetoothConnectionListener() {
            public void onDeviceConnected(String name, String address) {
                connect.setText("Connected to " + name);
                increment.setVisibility(View.VISIBLE);
                decrement.setVisibility(View.VISIBLE);
                reset.setVisibility(View.VISIBLE);
                count.setVisibility(View.VISIBLE);
                number.setVisibility(View.VISIBLE);
            }

            public void onDeviceDisconnected() {
                connect.setText("Connection lost");
            }

            public void onDeviceConnectionFailed() {
                connect.setText("Unable to connect");
            }
        });

        bt.setOnDataReceivedListener(new BluetoothSPP.OnDataReceivedListener(){
            public void onDataReceived(byte[] data, String message) {
                if (message.contains("count:")) {
                    message = message.substring(6);
                    String s = "";
                    for (int i = 0; i < message.length(); i++){
                        char c = message.charAt(i);
                        if (c >= 48 && c <=57) {
                            s += c;
                        } else {
                            break;
                        }
                    }
                    number.setText(s);
                }
            }
        });

        connect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (bt.getServiceState() == BluetoothState.STATE_CONNECTED) {
                    bt.disconnect();
                } else {
                    Intent intent = new Intent(getApplicationContext(), DeviceList.class);
                    startActivityForResult(intent, BluetoothState.REQUEST_CONNECT_DEVICE);
                    connect.setText("Connecting...");
                }
            }
        });

        increment.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                bt.send(INCREMENT, true);
            }
        });

        decrement.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                bt.send(DECREMENT, true);
            }
        });

        reset.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                bt.send(RESET, true);
            }
        });
    }

    public void onStart() {
        super.onStart();
        if (!bt.isBluetoothEnabled()) {
            bt.enable();
        } else {
            if (!bt.isServiceAvailable()) {
                bt.setupService();
                bt.startService(BluetoothState.DEVICE_OTHER);
            }
        }
    }


    public void onDestroy() {
        super.onDestroy();
        bt.stopService();
    }

    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == BluetoothState.REQUEST_CONNECT_DEVICE) {
            if (resultCode == Activity.RESULT_OK)
                bt.connect(data);
        } else if (requestCode == BluetoothState.REQUEST_ENABLE_BT) {
            if (resultCode == Activity.RESULT_OK) {
                bt.setupService();
            } else {
                Toast.makeText(getApplicationContext()
                        , "Bluetooth was not enabled."
                        , Toast.LENGTH_SHORT).show();
                finish();
            }
        }
    }
}
