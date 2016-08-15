using UnityEngine;
using System.Collections;
using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

public class GameController : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetButtonDown ("Jump")) {
			TestClient ();
		}
	}

	public void TestClient() {
		byte[] bytes = new byte[1024];

		try {
			IPHostEntry ipHostInfo = Dns.GetHostEntry("127.0.0.1");
			IPAddress ipAddress = ipHostInfo.AddressList[0];

			foreach (var d in ipHostInfo.AddressList){
				if (d.AddressFamily == AddressFamily.InterNetwork){
					ipAddress = d;		
				}
			}

			IPEndPoint remoteEP = new IPEndPoint(ipAddress, 11000);
			Debug.Log("remoteEP: " + remoteEP.ToString());

			// Create a TCP/IP  socket.
			Socket sender = new Socket(AddressFamily.InterNetwork, 
				SocketType.Stream, ProtocolType.Tcp );

			// Connect the socket to the remote endpoint. Catch any errors.
			try {
				sender.Connect(remoteEP);

				Debug.LogFormat("Socket connected to {0}",
					sender.RemoteEndPoint.ToString());

				// Encode the data string into a byte array.
				byte[] msg = Encoding.ASCII.GetBytes("This is a test<EOF>");

				// Send the data through the socket.
				//int bytesSent = 
					sender.Send(msg);

				// Receive the response from the remote device.
				int bytesRec = sender.Receive(bytes);
				Debug.LogFormat("Echoed test = {0}",
					Encoding.ASCII.GetString(bytes, 0, bytesRec));

				// Release the socket.
				sender.Shutdown(SocketShutdown.Both);
				sender.Close();

			} catch (ArgumentNullException ane) {
				Debug.LogFormat("ArgumentNullException : {0}", ane.ToString());
			} catch (SocketException se) {
				Debug.LogFormat("SocketException : {0}", se.ToString());
			} catch (Exception e) {
				Debug.LogFormat("Unexpected exception : {0}", e.ToString());
			}

		} catch (Exception e) {
			Debug.LogFormat( e.ToString());
		}
	}
}
