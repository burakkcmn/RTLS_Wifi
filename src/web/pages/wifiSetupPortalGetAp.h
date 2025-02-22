/*
 * wifiSetupPortalGetAp.h
 *
 *  Created on: Jan 3, 2023
 *      Author: aress
 */

#ifndef WEB_PAGES_WIFISETUPPORTALGETAP_H_
#define WEB_PAGES_WIFISETUPPORTALGETAP_H_

const char WIFISETUPPORTALGETAP_page[] PROGMEM = R"=====(
<!doctype html>
<html lang='en'>
	<head>
		<meta charset='utf-8'> <meta name='viewport' content='width=device-width, initial-scale=1'>
			<title>Wifi Setup</title> 
			<style>*,::after,::before{box-sizing:border-box;}
				body{margin:0;font-family:'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}
				.form-control{display:block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}
				button{cursor: pointer;border:1px solid transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;line-height:1.5;border-radius:.3rem;width:100%}
				.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}
				h1{text-align: center}
			</style> 
	</head> 

	<body>
		<main class='form-signin'> 
			<form action='/wifiapportal' method='post'> 
				<h1 class=''>Wifi Setup</h1>
				<br/>
				<div class='form-floating'>
					<label>SSID</label>
					<input type='text' class='form-control' name='ssid'> 
				</div>
				<div class='form-floating'>
					<br/>
					<label>Password</label>
					<input type='password' class='form-control' name='password'>
				</div>
				<div class='form-floating'>
					<br/>
					<label>Channel</label>
					<input type='text' class='form-control' name='channel'>
				</div>
				<div class='form-floating'>
					<br/>
					<label>Hidden</label>
					<input type='text' class='form-control' name='hidden'>
				</div>
				<div class='form-floating'>
					<br/>
					<label>Max Count</label>
					<input type='text' class='form-control' name='maxcount'>
				</div>
				<br/><br/>
				<button type='submit'>Save</button>
				<p style='text-align: right'><a href='/' style='color: #32C5FF'>Sing Up</a></p>
			</form>
		</main> 
	</body>
</html>
)=====";

#endif /* WEB_PAGES_WIFISETUPPORTALGETAP_H_ */
