using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;
using System.Threading;

public class ArduinoReader : MonoBehaviour
{
    
    SerialPort sp;
    public GameController gameController;

    Thread readThread;
    List<string> serialData = new List<string>();

    void Start()
    {
        string[] ports = SerialPort.GetPortNames();
        foreach(string port in ports){
            if(port.ToLower().Contains("usb") || port.ToLower().Contains("com")){
                sp = new SerialPort(port, 9600);
                break;
            }
        }
        
        if(sp != null){
            sp.Open();
            sp.ReadTimeout = 20;
            readThread = new Thread(Read);
            readThread.Start();
        }
    }

    public void Read()
    {
        while(true)
        {
            string message = "";
            try
            {
                while(true)
                {
                    string line = sp.ReadLine();
                    message += line;
                    if(line.Contains("e")){
                        gameController.ReceiveArduinoInput(message);
                        message = "";
                        break;
                    }
                }
            }
            #pragma warning disable
            catch (System.Exception ignored)
            {
                //  Debug.Log(e.Message);
            }
            #pragma warning restore
        }
    }

    public void Stop(){
        if(sp != null){
            readThread.Abort();
            sp.Close();
        }
    }

    void Update()
    {
        if(Input.GetKeyUp(KeyCode.Escape)){
            if(sp != null){
                readThread.Abort();
                sp.Close();
            }
        }
    }
}
