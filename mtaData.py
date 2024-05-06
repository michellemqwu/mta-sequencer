from nyct_gtfs import NYCTFeed
from datetime import datetime, date, time
from flask import Flask, jsonify

mta_api_key = "VETvY8w70Z5OfAge0SiKj1N93Dxz5ewA28yrdzzi"

app = Flask(__name__)

@app.route('/foo')
def getSubwayData():
    results = {}

    rTrainFeed = NYCTFeed("R", api_key=mta_api_key)
    rTrains = rTrainFeed.filter_trips(line_id=["R"], headed_for_stop_id=["A41","A41S","A41N", "R29", "R29S","R29N"], underway=True)
    rTrains = getClosestTrains(rTrains)
    rTrains = getTimeTillArrival(rTrains)
    for train in rTrains:
        if train[0] == 'R29S':
            results['rs'] = train[1]
        else:
            results['rn'] = train[1]
    
    aTrainFeed = NYCTFeed("A", api_key=mta_api_key)
    aTrains = aTrainFeed.filter_trips(line_id=["A"], headed_for_stop_id=["A41","A41S","A41N", "R29", "R29S","R29N"], underway=True)
    aTrains = getClosestTrains(aTrains)
    aTrains = getTimeTillArrival(aTrains)
    for train in aTrains:
        if train[0] == 'A41S':
            results['as'] = train[1]
        else:
            results['an'] = train[1]

    cTrainFeed = NYCTFeed("C", api_key=mta_api_key)
    cTrains = cTrainFeed.filter_trips(line_id=["C"], headed_for_stop_id=["A41","A41S","A41N", "R29", "R29S","R29N"], underway=True)
    cTrains = getClosestTrains(cTrains)
    cTrains = getTimeTillArrival(cTrains)
    for train in cTrains:
        if train[0] == 'A41S':
            results['cs'] = train[1]
        else:
            results['cn'] = train[1]

    fTrainFeed = NYCTFeed("F", api_key=mta_api_key)
    fTrains = fTrainFeed.filter_trips(line_id=["F"], headed_for_stop_id=["A41","A41S","A41N", "R29", "R29S","R29N"], underway=True)
    fTrains = getClosestTrains(fTrains)
    fTrains = getTimeTillArrival(fTrains)
    for train in fTrains:
        if train[0] == 'A41S':
            results['fs'] = train[1]
        else:
            results['fn'] = train[1]

    return jsonify(results), {"Access-Control-Allow-Origin": "*"}

def getClosestTrains(trains):
    all_data = []
    for train in trains:
            # for each (stop is an entry with the stop ID, arrival and departure times)
        for stop in train.stop_time_updates:
                # if it matches the stop we want (L08 is Bedford Ave L stop)
            if stop.stop_id == "A41S" or stop.stop_id == "A41N" or stop.stop_id == "R29S" or stop.stop_id == "R29N":
                stop_str = (str(stop))
                    # save the ID (so we know N vs S bound) and the arrival time
                all_data.append([stop.stop_id, stop_str[36:44]])

    all_data = sorted(all_data, key = lambda x: x[1])
    return all_data

def getTimeTillArrival(trains):
    to_return = []
    # this gets the next N and S trains
    for i in range(len(trains)):
        if i == 0:
            to_return.append(trains[i])
        else:
            if trains[i][0] != to_return[0][0]:
                to_return.append(trains[i])
                break
        
    time_list = []
        # now we have to get the time difference i.e. compare arrival time to what time it is now
    for j in range(len(to_return)):
            # get time string in form HH:MM:SS 
        datetime_str = to_return[j][1]
            # convert to datetime.time object
        time_obj = time.fromisoformat(datetime_str)
            # get current time as datetime.time object
        now = datetime.now().time()
            #return([str(now), str(time_obj)])
            # get difference (not sure if this is doing what I think it's doing...)
        time_difference = datetime.combine(date.today(), time_obj) - datetime.combine(date.today(), now)
            # format to string so we can parse
        time_difference = str(time_difference)
            # get minutes (this does not give true time difference, need to account for hours as well)
        k = time_difference.find(":")
        minutes_until_train = time_difference[k+1:k+3]
        time_list.append([to_return[j][0], minutes_until_train])
    return time_list

app.run(host='0.0.0.0', port= 8090)