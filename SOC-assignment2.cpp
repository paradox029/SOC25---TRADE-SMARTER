#include<iostream>
#include<vector>

using namespace std;

float EMA_calculator(int period , vector <float> &closing_prices) {
    float multiplier = 2.0/(period + 1);
    float ema =0.0;
    for(int i = (closing_prices.size()-1)-2*(period-1) ; i <= closing_prices.size()-period ; i++) {
        ema += closing_prices[i]; // Calculate the initial EMA as the average(SMA) of the first 'period' closing prices
    }
    ema /= period; // Initial EMA is the average of the first 'period' closing prices
    vector <float> period_prices;
    for(int i=(closing_prices.size()-1)-(period-1); i < closing_prices.size(); i++) {
        period_prices.push_back(closing_prices[i]); // Get the last 'period' closing prices
    }
    for (int i = 0; i < period_prices.size(); i++) {
        ema = (closing_prices[i] - ema) * multiplier + ema;
    }
    return ema; 
}

float MACD_calculator(vector<float> &closing_prices, int short_period, int long_period) {
    float short_ema = EMA_calculator(short_period, closing_prices);
    float long_ema = EMA_calculator(long_period, closing_prices);
    return short_ema - long_ema;
}

float RSI_calculator(vector<float> &closing_prices, int period){
    float gain = 0.0, loss = 0.0;
    vector <float> period_prices;
    for(int i = closing_prices.size()-1-period; i < closing_prices.size(); i++) {
        period_prices.push_back(closing_prices[i]); // Get the last 'period' closing prices
    }
    for(int i = 1; i < period_prices.size(); i++) {
        float change = period_prices[i] - period_prices[i-1];
        if (change > 0) {
            gain += change;
        }
        else {
            loss -= change; // loss is negative, so we subtract
        }
    }
    float RSI = loss==0? 100:  100 - (100 / (1 + gain / loss));
    return RSI; 
}

string simple_EMA_Crossover(vector<float> &closing_prices, int short_period , int long_period){
    float short_ema = EMA_calculator(short_period, closing_prices);
    float long_ema = EMA_calculator(long_period, closing_prices);
    if (short_ema > long_ema) {
        return "Bullish " + to_string(short_ema) + " " + to_string(long_ema);
    } else if (short_ema < long_ema) {
        return "Bearish " + to_string(short_ema) + " " + to_string(long_ema);
    } else {
        return "Possible imminent crossover " + to_string(short_ema) + " " + to_string(long_ema);
    }
}

string MACD_strat(vector <float> &closing_prices, int signal_period = 9 , int short_period = 12 , int long_period = 26) {
    vector<float> macd_values (signal_period, 0.0); // Initialize MACD values with zeros
    vector<float> closing_prices_copy = closing_prices;
    for(int i = signal_period-1 ; i>=0; i--) {
        macd_values[i] =MACD_calculator(closing_prices_copy, short_period, long_period);
        closing_prices_copy.pop_back(); // Remove the last element to simulate moving window

    }
    float signal_line = EMA_calculator(signal_period, macd_values);
    float last_macd = macd_values.back();
    if (last_macd > signal_line) {
        return "Bullish " + to_string(last_macd) + " " + to_string(signal_line);
    } 
    else if (last_macd < signal_line) {
        return "Bearish " + to_string(last_macd) + " " + to_string(signal_line);
    } 
    else {
        return "Possible imminent crossover " + to_string(last_macd) + " " + to_string(signal_line);
    }
}

string RSI_strat(vector<float> &closing_prices, int period = 14) {
    float rsi = RSI_calculator(closing_prices, period);
    if (rsi < 20){
        return "Oversold - Buy Signal " + to_string(rsi);
    } 
    else if (rsi > 20 && rsi<40) {
        return "Bearish - Can short if trendy market is present " + to_string(rsi);
    } 
    else if (rsi > 60 && rsi < 80) {
        return "Bullish - Can long if trendy market is present " + to_string(rsi);
    }
    else if (rsi > 80) {
        return "Overbought - Sell Signal " + to_string(rsi);
    } 
    else {
        return "No clear signal " + to_string(rsi);
    }
}

int main() {
    vector<float> closing_prices = {100, 102, 101, 105, 107, 110, 108, 111, 115, 120,100, 102, 101, 105, 107, 110, 108, 111, 115, 120,100, 102, 101, 105, 107, 110, 108, 111, 115, 120,100, 102, 101, 105, 107, 110, 108, 111, 115, 120,100, 102, 101, 105, 107, 110, 108, 111, 115, 120,100, 102, 101, 105, 107, 110, 108, 111, 115, 120,100, 102, 101, 105, 107, 110, 108, 111, 115, 120, 100, 102, 101, 105, 107, 110, 108, 111, 115, 120};
    int short_period , long_period;
    cout << "Enter short EMA period: ";
    cin >> short_period;
    cout << "Enter long EMA period: ";
    cin >> long_period;
    cout << "EMA Crossover Signal: " << simple_EMA_Crossover(closing_prices, short_period, long_period) << endl;
    cout << "RSI Strategy Signal: " << RSI_strat(closing_prices) << endl;
    cout << "MACD Strategy Signal: " << MACD_strat(closing_prices) << endl;
}
