/*
 * wifiSetupPortalGet.h
 *
 *  Created on: Dec 29, 2022
 *      Author: aress
 */

#ifndef WEB_PAGES_WIFISETUPPORTALGETBODY_H_
#define WEB_PAGES_WIFISETUPPORTALGETBODY_H_

const char WIFISETUPPORTALGETST_BODY_page[] PROGMEM = R"=====(
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

#endif /* WEB_PAGES_WIFISETUPPORTALGETBODY_H_ */
