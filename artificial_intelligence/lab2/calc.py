percent = 0.055
invested = 700000
inflation_adjusted = True
inflation = 0.03


profit_per_year = invested * percent * 0.81
if not inflation_adjusted:
    print(profit_per_year, profit_per_year/12)
else:
    returned_profit = invested * inflation * 0.81
    total = profit_per_year + returned_profit
    print(profit_per_year, returned_profit, total, total/12)
