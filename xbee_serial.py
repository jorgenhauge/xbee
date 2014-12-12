import serial
import time
import datetime
import urllib
import urllib2

ACCESS_TOKEN = 'xsjKNhrX8AcS2GSB1-lvGhK1hdEbsIdgaBWVIIFW4OTUuEyLTVNankTWnZbJWtiduLpATyQI9cw='
PROJECT_ID = '2d880520812711e4bf2122000a9e07fe'
ANALOG_TEMP_SENSOR = 'VTMP1'
ID = 'ID'

class StormLog(object):

    def __init__(self, access_token, project_id, input_url=None):
        self.url = input_url or 'https://api-2n6b-uxaj.data.splunkstorm.com/1/inputs/http'
        self.project_id = project_id
        self.access_token = access_token

        self.pass_manager = urllib2.HTTPPasswordMgrWithDefaultRealm()
        self.pass_manager.add_password(None, self.url, 'x', access_token)
        self.auth_handler = urllib2.HTTPBasicAuthHandler(self.pass_manager)
        self.opener = urllib2.build_opener(self.auth_handler)
        urllib2.install_opener(self.opener)

    def send(self, event_text, sourcetype='syslog', host=None, source=None):
        params = {'project': self.project_id,
                  'sourcetype': sourcetype}
        if host:
            params['host'] = host
        if source:
            params['source'] = source
        url = '%s?%s' % (self.url, urllib.urlencode(params))
        try:
            req = urllib2.Request(url, event_text)
            response = urllib2.urlopen(req)
            return response.read()
        except (IOError, OSError), ex:
            # An error occured during URL opening or reading
            raise


def reading_to_temp(reading, celsius=True):
    voltage = reading * 5.0
    float(voltage)
    voltage /= 1024.0
  # (type == "Far"){
  #   return(temperature * 9.0 / 5.0) + 32.0;
  # }*/
    return (voltage - 0.5) * 100


def get_data(ser):

    def to_dict(raw_data):
        splitted = raw_data.split() # data: 'ID: 40BEF4EA VTMP1: 155\r\n'
        #Splitted: ['ID:', '40BEF4EA', 'VTMP1:', '155']
        try:
            converted = {splitted[i]: splitted[i+1] for i in range(0, len(splitted), 2)}
        except IndexError as e:
            raise "Could not split data: {}, {}".format(splitted, e)
        return converted

    def write(character="T"):
        try:
            ser.write(character) # Write data("T") to device to get temp
        except Exception as e:
            print e

    def read():
        data = ser.readline()
        if data == '':
            raise Exception("No data received on {}, data is: {}".format(ser.port, data))
        return data
        
    write()
    data = read()
    return to_dict(data) # Get data from serial line


if __name__ == '__main__':
    # Example
    # Setup the example logger
    # Arguments are your access token and the project ID
    log = StormLog(ACCESS_TOKEN, PROJECT_ID)
    # Send a log; will pick up the default value for ``source``.
    try:
        ser = serial.Serial('/dev/ttyUSB2', 9600, timeout=1)
    except Exception as e:
        print e
    while True:
        now = datetime.datetime.now()
        time_capture = now.strftime("%A, %d %B %Y %I:%M:%S")
        data_dict = get_data(ser)
        # {'ID': '40BEF4EA', 'VTMP1': '159'}
        sensor_value = data_dict.get('VTMP1', 0)
        sensor_value = float(sensor_value)
        print sensor_value
        sensor_temp = reading_to_temp(sensor_value)
        sensor_id = data_dict.get(ID)
        print sensor_id

        # export ACCESS_TOKEN=<access_token>
        # export PROJECT_ID=<project_id>
        # export SOURCETYPE=generic_single_line
        # export EVENT="Sun Apr 11 15:35:15 UTC 2011 action=download_packages status=OK pkg_dl=751 elapsed=37.543"

        # curl -u x:$ACCESS_TOKEN \
        #   "https://api-****.data.splunkstorm.com/1/inputs/http?index=$PROJECT_ID&sourcetype=$SOURCETYPE" \
        #   -H "Content-Type: text/plain" \
        #   -d "$EVENT"



        # log.send('Apr 1 2012 18:47:23 UTC host57 action=supply_win amount=5710.3',
        #          sourcetype='generic_single_line')
        # # Will pick up the 'default' value for ``host``.
        # log.send('Apr 1 2012 18:47:26 UTC host44 action=deliver from=foo@bar.com to=narwin@splunkstorm.com',
        #          sourcetype='syslog')
        try:
            log.send(event_text='temperature={}'.format(sensor_temp),
                     sourcetype='temperature_data',
                     host='johauge-laptop',
                     source='{}'.format(sensor_id))
            print "Sent ID={} TEMP={}, {} to SPLUNK".format(sensor_id, sensor_temp, time_capture)
        except Exception as e:
            print 'Got an exception:', repr(e)
        # Will pick up the 'default' value for ``host``.
        
        time.sleep(30)





