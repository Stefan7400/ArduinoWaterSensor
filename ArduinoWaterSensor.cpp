#define SensorPin A3            
#define Offset 0.04            
#define first_ph_reference_voltage 1.73
#define first_standard_solution 6.7
#define second_ph_reference_voltage 1.29
#define second_standard_solution 4.1
#define LED 13
#define samplingInterval 1000
#define printInterval 1000
#define ArrayLenth  40    
int pHArray[ArrayLenth];   
int pHArrayIndex = 0;
void setup(void)
{
    pinMode(LED, OUTPUT);
    Serial.begin(115200);
    Serial.println("pH meter experiment!");    //Output on serial monitor
}

void loop(void)
{
    static bool up = false;
    static bool down = false;
    static unsigned long samplingTime = millis();
    static unsigned long printTime = millis();
    static float pHValue, voltage;
    if (millis() - samplingTime > samplingInterval)
    {
        pHArray[pHArrayIndex++] = analogRead(SensorPin);
        if (pHArrayIndex == ArrayLenth)pHArrayIndex = 0;
        voltage = avergearray(pHArray, ArrayLenth) * 5.0 / 1024;
        float slope = (first_standard_solution - second_standard_solution) / ((first_ph_reference_voltage - 1.5) / 3 - (second_ph_reference_voltage - 1.5) / 3);
        float intercept = first_standard_solution - slope * (first_ph_reference_voltage - 1.50) / 3;
        pHValue = slope * (voltage - 1.5) / 3 + intercept + Offset;
        samplingTime = millis();
    }
    if (millis() - printTime > printInterval)   //Every 800 milliseconds
    {
        Serial.print("Voltage:");
        Serial.print(voltage, 2);
        Serial.print("    pH value: ");
        Serial.println(pHValue, 2);
        digitalWrite(LED, digitalRead(LED) ^ 1);
        printTime = millis();
    }
}
double avergearray(int* arr, int number) {
    int i;
    int max, min;
    double avg;
    long amount = 0;
    if (number <= 0) {
        Serial.println("Error number for the array to avraging!/n");
        return 0;
    }
    if (number < 5) {   //less than 5, calculated directly statistics
        for (i = 0; i < number; i++) {
            amount += arr[i];
        }
        avg = amount / number;
        return avg;
    }
    else {
        if (arr[0] < arr[1]) {
            min = arr[0]; max = arr[1];
        }
        else {
            min = arr[1]; max = arr[0];
        }
        for (i = 2; i < number; i++) {
            if (arr[i] < min) {
                amount += min;        //arr<min
                min = arr[i];
            }
            else {
                if (arr[i] > max) {
                    amount += max;    //arr>max
                    max = arr[i];
                }
                else {
                    amount += arr[i]; //min<=arr<=max
                }
            }//if
        }//for
        avg = (double)amount / (number - 2);
    }//if
    return avg;
}