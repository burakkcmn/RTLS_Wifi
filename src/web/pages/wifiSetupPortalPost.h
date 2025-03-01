/*
 * asd.h
 *
 *  Created on: Dec 29, 2022
 *      Author: aress
 */

#ifndef WEB_PAGES_WIFISETUPPORTALPOST_H_
#define WEB_PAGES_WIFISETUPPORTALPOST_H_

const char WIFISETUPPORTALPOST_page[] PROGMEM = R"=====(
<!doctype html>
	<html lang='en'>
		<head>
			<meta charset='utf-8'>
				<meta name='viewport' content='width=device-width, initial-scale=1'>
					<title>Wifi Setup</title>
					<style>*,::after,::before{box-sizing:border-box;}
						body{margin:0;font-family:'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}
							.form-control{display:block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}
							button{border:1px solid transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;line-height:1.5;border-radius:.3rem;width:100%}
							.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1,p{text-align: center}
					</style> 
		</head> 
		<body>
			<main class='form-signin'> 
				<h1>Wifi Setup</h1> 
				<br/> <p>Your settings have been saved successfully!
				<br />Please restart the device.</p>
			</main>
		</body>
	</html>
)=====";

#endif /* WEB_PAGES_WIFISETUPPORTALPOST_H_ */
