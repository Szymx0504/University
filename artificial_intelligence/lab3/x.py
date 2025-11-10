capital = 3000
add_per_month = 500
surplus_year = 0.15
surplus_month = surplus_year / 12
revenue = 0
YEARS = 40
money_spent = capital
monthly_incomes = []
for i in range(12 * YEARS):
    if i > 60:
        add_per_month = 2500

    monthly_plus = capital * surplus_month
    revenue += monthly_plus # zysk - dla ciebie
    capital += add_per_month # dopłacanie
    money_spent += add_per_month
    monthly_incomes.append(monthly_plus)
    capital += monthly_plus

print(money_spent, int(revenue), int(money_spent + revenue))
print(capital * surplus_month)
print(monthly_incomes[120])