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
		void Set_dynamicVariable2 (String value);
		String Get_dynamicVariable2 ();
		void Set_dynamicVariable (String value);
		String Get_dynamicVariable ();
	private:
		void Submit_Callback(void);
		void(*submit_UserCallback)(void);
		ESP8266WebServer* server;

		String dynamicVariable2;
		String dynamicVariable;
};