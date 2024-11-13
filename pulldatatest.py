import requests
import json

def get_data_from_think_speak(start_date = '2023-11-25%2000:00:00', end_date = '2023-11-26%2021:00:00'):
    try:
        CHANNEL_ID = '2685862'
        READ_API_KEY = 'KU3N5AZKUWI7SO03'

        # url = f'https://api.thingspeak.com/channels/{CHANNEL_ID}/feeds.json?api_key={READ_API_KEY}&start={start_date}&end={end_date}'
        url = f'https://api.thingspeak.com/channels/{CHANNEL_ID}/feeds.json?api_key={READ_API_KEY}'
        response = requests.get(url)

        if response.status_code == 200:
            data = response.json()

            # Extract the list of entries
            entries = data['feeds']

            # Create a dictionary to store data

            data_dict = {'field1': entries[0]["field1"]}

            return entries[len(entries)-1]["field1"]
        else:
            print(f"Error {response.status_code}: {response.text}")
       
    except:
        return json.dumps({"message":"Error occur"})

def get_all_data_from_think_speak(start_date = '2023-11-25%2000:00:00', end_date = '2023-11-26%2021:00:00'):
    try:
        CHANNEL_ID = '2685862'
        READ_API_KEY = 'KU3N5AZKUWI7SO03'

        # url = f'https://api.thingspeak.com/channels/{CHANNEL_ID}/feeds.json?api_key={READ_API_KEY}&start={start_date}&end={end_date}'
        url = f'https://api.thingspeak.com/channels/{CHANNEL_ID}/feeds.json?api_key={READ_API_KEY}'
        response = requests.get(url)

        if response.status_code == 200:
            data = response.json()

            # Extract the list of entries
            entries = data['feeds']

            # Create a dictionary to store data
            data_dict = {'entries': []}

            # Add each entry to the dictionary
            for entry in entries:
                entry_data = {
                    'field1': entry['field1']
                }
                data_dict['entries'].append(entry_data)
            # data_dict = data_dict[::-1]
            return json.dumps(data_dict)
        else:
            print(f"Error {response.status_code}: {response.text}")
       
    except:
        return json.dumps({"message":"Error occur"})
