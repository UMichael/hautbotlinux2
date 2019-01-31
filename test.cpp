#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <curl/curl.h>
#include <string>
#include <python3.6m/Python.h>
#include <thread>
using namespace std;
int internet();
void disconnect();
void selenium(bool, bool , int &, bool &);
int ping();
CURL *curl= curl_easy_init();

int main(){
    Py_Initialize();
	//Initialize all functions before start so as not to waste work flow time
	PyRun_SimpleString("from selenium import webdriver\n"
		"from xvfbwrapper import Xvfb\n"
		"display = Xvfb()\n"
		"from selenium.webdriver.support.ui import Select\n"
		"from selenium.webdriver.chrome.options import Options\n"
		"chrome_options = Options()\n"
		"chrome_options.Proxy = None\n"
		//"chrome_options.add_argument('--disable-gpu')\n"
		"chrome_options.add_argument('--ignore-ssl-errors')\n"			
		"chrome_options.add_argument('--incognito')\n"

		"prefs = {'profile.default_content_setting_values': { 'images': 2, 'javascript': 2, 'plugins': 2,"
		"'popups' : 2, 'geolocation' : 2,'notifications' : 2, 'auto_select_certificate' : 2, 'fullscreen' : 2,"
		"'mouselock' : 2, 'mixed_script' : 2, 'media_stream' : 2, 'media_stream_mic' : 2, 'media_stream_camera' : 2,"
		"'protocol_handlers' : 2,'ppapi_broker' : 2, 'automatic_downloads' : 2, 'midi_sysex' : 2,'push_messaging' : 2,"
		"'ssl_cert_decisions' : 2, 'metro_switch_to_desktop' : 2,'protected_media_identifier' : 2, 'app_banner' : 2, "
		"'site_engagement' : 2,'durable_storage' : 2}}\n"
		"chrome_options.add_experimental_option( 'prefs',prefs )\n"
		"chrome_options.add_argument('--disable-infobars')\n"
		"chrome_options.add_argument('--disable-extensions')\n"
	);
    std::cout<<"Press 1 for Fast and 2 for Slow"<<endl;
    int value;
    std::cin>>value;
    bool fast;
    if (value ==1)
        fast=true;
	else
	{
		fast=false;
	}
	
    bool isup=false;
    bool uselessbool=false;
    int count=0;
    int uselessint=1;
	disconnect();
	int check = ping();
    while(true){
        disconnect();
        if (check==1||check==256||check==512){  //If network is down
            std::cout<<"Network is down "<<endl;
            std::cout<<"\nHold on please"<<endl;
            std::cout<<"Enabling HOLD ON"<<endl<<endl;
           
            if(fast)
            {
				thread imacros(selenium,true,true,ref(check),ref(isup));
            	imacros.detach();
            	while (isup == false) {
					cout << "Hold on pls TESTING" << endl;
					check = ping();
				}
            	if (check == 0) {
					isup = true;
                    cout<<"There was a glitch but its sorted"<<endl;
					continue;
				}
                std::cout<<"Performing ADB action"<<endl<<endl;
                std::system("~/Desktop/adb.sh");
				disconnect();
				check=ping();
				if (check==1||check==256||check==512) {
			    	isup = false;
				}
            }
            else
            {
				selenium(true,false,uselessint,uselessbool);
                check=ping();
                while(check==1||check==256||check==512)
                {
                    std::cout<<"Hold on for network to come up"<<endl;
                    check=ping();
                }
                disconnect();
            }
            disconnect();
            continue;
        }
        else if(check==0)
        {
            cout << "Network is up" << endl << endl;
			//if network is up at first
			if (count == 0)
				isup = true;
			while (isup == false) {
				cout << "Waiting for disallow to end" << endl;
				if (check==0){
					isup=true;
					check=ping();
				}
				usleep(1000000);
			}
			cout << "Hold on please DISALLOWING" << endl << endl;
			selenium(false, false, uselessint, uselessbool);
			//this reinitialize isup but really not needed since if we run allow or disallow at first isup become false then after successful run isup becomes true
			isup = false;
			count = 1;
        }
        while(check==0)
        {
            cout<<"Network UP"<<endl<<endl;
            usleep(10000000);
            disconnect();
			check=ping();
        }
    }
    Py_Finalize();
    //cout<<res;
	curl_easy_cleanup(curl);
    std::cout<<"Press any enter to continue...."<<endl;
    std::cin.get();
}
int ping()
{
    return std::system("ping -W 2 -c 2 google.com");
}

int internet(){
    CURLcode res;
    if(curl) {

        curl_easy_setopt(curl, CURLOPT_URL, "154.66.2.12/scp");
        curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT_MS, 5000L);
        curl_easy_setopt(curl, CURLOPT_NOBODY,1);
        res = curl_easy_perform(curl);
        string word="No error";
        if (curl_easy_strerror(res)==word){
            return 0;
        }
        if (res == CURLE_OPERATION_TIMEDOUT){
            cout<<"There was a timeout"<<endl;
            // curl_easy_cleanup(curl);
            // curl_easy_setopt(curl, CURLOPT_URL, "www.smile.com.ng/scp");
            // res = curl_easy_perform(curl);
            // if (curl_easy_strerror(res)==word){
            //     return 1;
            // }
            return 408; //timedout if there was a call or network error
            
        }
        switch (res)
        {
            case CURLE_COULDNT_CONNECT:
                return 204;//in case of no internet
            case CURLE_COULDNT_RESOLVE_HOST:
                cout<<"COunldnt resolve";
                break;
            case CURLE_COULDNT_RESOLVE_PROXY:
                cout<<"Internet dose not exist";
                break;
            case CURLE_NO_CONNECTION_AVAILABLE:
                cout<<"connection";
                break;
            default:
                return 400; //This is an unknow error disable and shutdown internet if possible
        }
    }
    return 1;
}

void disconnect(){
    while(internet()!=0){
        cout<<"Network seems to be down please confirm"<<endl;
    }
    return;
}

void selenium(bool allow, bool fast, int &check, bool &vals)
{
	vals = false;
	
	int test = 1;
	while (test != 0) {
		test = PyRun_SimpleString(
			
			"display.start()\n"
			"driver = webdriver.Chrome(executable_path = '/home/michael/Desktop/chromedriver', options = chrome_options)\n"
			"print('HEllo')\n");
		if (test != 0)
		{
			PyRun_SimpleString("driver.close()\n"
				"driver.quit()\n");
			continue;
		}
		cout << test;
		if (fast && allow) {
			test = PyRun_SimpleString(
				"print('testing driver')\n"
				"driver.get('http://154.66.2.11/scp/login.action')\n"
				
				"fillName = driver.find_element_by_name('username')\n");
			if (test != 0)
			{
				PyRun_SimpleString("driver.close()\n"
					"driver.quit()\n");
				continue;
			}
		}
		else {
			test = PyRun_SimpleString(
				"driver.get('https://smile.com.ng/scp/Login.action')\n"
				"fillName = driver.find_element_by_name('username')\n");
			if (test != 0)
			{
				PyRun_SimpleString("driver.close()\n"
					"driver.quit()\n");
				continue;
			}
		}
		cout<<endl<<"filled username here"<<endl<<test<<endl<<endl;;

		
		std::string val = "fillName.send_keys('utimichael9')\n";
		test = PyRun_SimpleString(val.c_str());
		if (test != 0)
		{
			PyRun_SimpleString("driver.close()\n"
				"driver.quit()\n");
			continue;
		}
		test = PyRun_SimpleString("fillPassword = driver.find_element_by_name('password')\n");
		if (test != 0)
		{
			PyRun_SimpleString("driver.close()\n"
				"driver.quit()\n");
			continue;
		}
		val = "fillPassword.send_keys('put your password here')";
		test = PyRun_SimpleString(val.c_str());
		if (test != 0)
		{
			PyRun_SimpleString("driver.close()\n"
				"driver.quit()\n");
			continue;
		}
		cout<<endl<<"filled password here"<<endl<<test<<endl<<endl;
		test = PyRun_SimpleString(
			"clic = driver.find_element_by_name('login')\n"
			"clic.click()\n"
			"select = Select(driver.find_element_by_name('account.status'))\n"
		);
		if (test != 0)
		{
			PyRun_SimpleString("driver.close()\n"
				"driver.quit()\n");
			continue;
		}
		if (allow)
		{
			test = PyRun_SimpleString("select.select_by_value('0')\n");
			if (test != 0)
			{
				PyRun_SimpleString("driver.close()\n"
					"driver.quit()\n");
				continue;
			}
		}
		else
		{
			test = PyRun_SimpleString("select.select_by_value('14')\n");
			if (test != 0)
			{
				PyRun_SimpleString("driver.close()\n"
					"driver.quit()\n");
				continue;
			}
		}
		cout<<"\n about checking if data is up"<<endl<<test<<endl<<endl;;
		if (check == 0 && allow) {
			PyRun_SimpleString("driver.close()\n"
				"driver.quit()\n");
			break;
		}
		test = PyRun_SimpleString(
			"pressOK = driver.find_element_by_name('changeAccountStatus')\n"
			"pressOK.click()\n"
			"driver.delete_all_cookies()\n"
			"driver.close()\n"
			"driver.quit()\n"
		);
		cout<<"Done with clicking"<<endl<<test<<endl<<endl;
		if (test != 0)
		{
			PyRun_SimpleString("driver.close()\n"
			"driver.quit()\n");
			continue;
		}
	}
	PyRun_SimpleString(
		"display.stop()\n"
	);
	
	vals = true;
	cout<<"vals is "<<vals<<endl;
}