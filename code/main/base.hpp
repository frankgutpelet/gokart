#include <ESP8266WebServer.h>
#include <Arduino.h>
#pragma once
class base
{
	public:
		base (ESP8266WebServer* server);

		void Render (void);
		void GetAjaxValues (void);
		void SetCallback_submit (void (*callback)(void));
		void Set_minValue (String value);
		String Get_minValue ();
		void Set_trottle (String value);
		String Get_trottle ();
		void Set_maxValue (String value);
		String Get_maxValue ();
		void Set_action (String value);
		String Get_action ();
	private:
		void Submit_Callback(void);
		void(*submit_UserCallback)(void);
		ESP8266WebServer* server;

		String minValue;
		String trottle;
		String maxValue;
		String action;
};
