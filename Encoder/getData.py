import sys, serial, time, csv

# Creates an array of numbers based on the string (a,b,c)
# If there's any problem on the string, returns (_,False)
def parseEncoder(s):
    data = []
    success = True

    for number in s.split(","):
        try:
            data.append(int(number))
        except ValueError:
            success = False

    if(len(data) != 3):
        success = False
    return (data, success)

if __name__ == '__main__':
    ## Establish connection to the serial port that your teensy
    ## is connected to.
    serialPorts=['COM13']
    for port in serialPorts:
        try:
            print "Trying to connect to Teensy on",port
            device = serial.Serial(port, 115200, timeout=2)
            break
        except:
            print "Failed to connect on ",port
            exit(1)

    print "Teensy is ready!"

    numberOfSamples = 0
    currentEncoder = [0,0,0]
    auxString = ""
    startTime = time.time()
    outputFile = open('data.csv', 'wb')
    csvWriter = csv.writer(outputFile)

    # Keep reading everything sent by the teensy
    while 1:
        # Verifies if there's something to read in the buffer
        if device.inWaiting():

            # Reads a character
            c = device.read()

            # If its not the end of the line yet, keep appending chars to the string
            if(c != '\r' and c != '\n'):
                auxString = auxString+str(c)
            # If we reached the end of the line, parse the numbers read
            elif (c == '\n'):
                (data, success) = parseEncoder(auxString)
                if(success):
                    numberOfSamples = numberOfSamples+1
                    currentEncoder = data

                auxString = ""

                output = [time.time() - startTime] + currentEncoder

                print output
                # write to csv
                csvWriter.writerow(output)

    device.close()
    outputFile.close()
