
// ^~^~^~^~^^~^~^~^~^ Edited equations begin here ^~^~^~^~^^~^~^~^~^

/**********************************************************************
Bank Stocks and Flows
Dawid e Delli Gatti (2018), item 2.2.
*********************************************************************/


/*
Equation for "Bank_Stock_Loans_Short_Term" was moved to the final portion of this file, since is part of the unedited codes from the MMM (v3.9), as written by Matheus Vianna
*/

EQUATION("Bank_Stock_Loans_Short_Term")
//Total Stock of short term loans, firms and classes


	v[10]=V("bank_id");

	v[0]=0;
	CYCLES(root, cur, "SECTORS")
	{
		v[1]=0;
		CYCLES(cur, cur1, "FIRMS")
		{
			v[2]=VS(cur1, "Firm_Stock_Loans_Short_Term");
			v[3]=VS(cur1, "firm_bank");
			
			if(v[3]==v[10])
				v[1]=v[1]+v[2];
			else
				v[1]=v[1];
		}
		v[0]=v[0]+v[1];
	}
	
	v[4]=0;
	CYCLES(root, cur, "CLASSES")
		{
		v[5]=0;
		CYCLES(cur, cur1, "CLASS_LOANS")
			{
			v[6]=VS(cur1, "class_loan_total_amount");
			v[7]=VS(cur1, "class_loan_bank_id");
			
			if(v[7]==v[10])
				v[5]=v[5]+v[6];
			else
				v[5]=v[5];
			}
		v[4]=v[4]+v[5];
	}
	v[8]=v[0]+v[4];
	
RESULT(v[8])


EQUATION("Bank_Stock_Deposits")
// Stock of deposits from firms and households (bank liabilities)
	
	v[10]=V("bank_id");

	v[0]=0;
	CYCLES(root, cur, "SECTORS")
	{
		v[1]=0;
		CYCLES(cur, cur1, "FIRMS")
		{
			v[2]=VS(cur1, "Firm_Stock_Deposits");
			v[3]=VS(cur1, "firm_bank");
			if(v[3]==v[10])
				v[1]=v[1]+v[2];
			else
				v[1]=v[1];
		}
		v[0]=v[0]+v[1];
	}
	
	v[4]=0;
	CYCLES(root, cur, "CLASSES")
	{
		v[5]=VS(cur, "Class_Stock_Deposits");
		v[4]=v[4]+v[5];
	}
	v[6]=VL("Bank_Market_Share", 1);
	v[7]=v[0]+v[6]*v[4];
RESULT(v[7])


EQUATION("Bank_Flow_Loans")
// Loan remuneration (from firms and households to banks)

	v[10]=V("bank_id");

	v[0]=0;
	CYCLES(root, cur, "SECTORS")
	{
		v[1]=0;
		CYCLES(cur, cur1, "FIRMS")
		{
			v[2]=VS(cur1, "Firm_Interest_Payment");
			v[3]=VS(cur1, "firm_bank");
			if(v[3]==v[10])
				v[1]=v[1]+v[2];
			else
				v[1]=v[1];
		}
		v[0]=v[0]+v[1];
	}
	
	v[4]=0;
	CYCLES(root, cur, "CLASSES")
		{
		v[5]=0;
		CYCLES(cur, cur1, "CLASS_LOANS")
			{
			v[6]=VS(cur1, "class_loan_total_amount");
			v[7]=VS(cur1, "class_loan_interest_rate");
			v[8]=VS(cur1, "class_loan_bank_id");
			
			if(v[8]==v[10])
				v[5]=v[5]+(v[6]*v[7]);
			else
				v[5]=v[5];
			}
		v[4]=v[4]+v[5];
	}
	v[9]=v[0]+v[4];
	
RESULT(v[9])


EQUATION("Bank_Flow_Deposits")
// Deposit remuneration (from banks to firms and households)

	v[10]=V("bank_id");

	v[0]=0;
	CYCLES(root, cur, "SECTORS")
	{
		v[1]=0;
		CYCLES(cur, cur1, "FIRMS")
		{
			v[2]=VS(cur1, "Firm_Deposits_Return");
			v[3]=VS(cur1, "firm_bank");
			if(v[3]==v[10])
				{v[1]=v[1]+v[2];}
			else
				{v[1]=v[1];}
		}
		v[0]=v[0]+v[1];
	}
	
	v[4]=0;
	CYCLES(root, cur, "CLASSES")
	{
		v[5]=VS(cur, "Class_Deposits_Return");
		v[4]=v[4]+v[5];
	}
	v[6]=VL("Bank_Market_Share", 1);
	v[7]=v[0]+v[6]*v[4];
	
RESULT(v[7])


/*
Equations for "Bank_Defaulted_Loans" and "Bank_Stock_Defaulted_Loans" were moved to the final portion of this file, since they are unedited codes from the MMM (v3.9), as prepared by Matheus Vianna
*/


EQUATION("Bank_Profits")
// Current bank profits.

	v[0]=V("Bank_Flow_Loans");
	v[1]=V("Bank_Flow_Deposits");
	v[2]=V("Bank_Defaulted_Loans");
	
	v[4]=v[0]-v[1]-v[2];											// Operational result (excl. govt operations)
	v[5]=V("Government_Interest_Payment");        
	v[6]=VL("Bank_Market_Share", 1);
	v[7]=v[4]+(v[5]*v[6]);												// Operational result (incl. govt operations)
RESULT(v[7])


EQUATION("Bank_Retained_Profits")
// Bank profits after tax and dividend distribution (cf. van der Hoog & Dawid (2016), 3.11.2.)

	v[1]=V("Bank_Profits");
	v[2]=V("bank_tax_rate");											// (Parameter) Bank's income tax
	v[3]=V("bank_dividend_rate");									// (Parameter) Bank's dividend distribution rate
	if (v[1] > 0)
		{
		v[4]=(1-v[2])*v[1];												// If profits are positive, bank pays income taxes at fixed rate and...
		v[5]=(1-v[3])*v[4];														// ... distributes dividends to shareholders.
		v[6]=v[3]*v[4]; 
		}
	else
		{
		v[5]=v[1];
		v[6]=0;
		}

WRITE("Bank_Distributed_Profits", v[6]);
RESULT(v[5])

EQUATION_DUMMY("Bank_Distributed_Profits", "Bank_Retained_Profits")


EQUATION("Bank_Accumulated_Profits")
// Bank's accumulated profits (equivalent to Net Worth in this case)

v[0]=VL("Bank_Accumulated_Profits",1);
v[1]=V("Bank_Retained_Profits");
v[2]=v[1]+v[0];

if(v[2]<0)
{
	v[3]=V("cb_minimum_capital_ratio");
	v[4]=VL("Bank_RWAcpad", 1);								// ** included "Bank_Credit_Risk", instead of "Bank_Total_Stock_Loans", as originally stated **
	v[5]=v[4]*v[3];													//minimal capital required to current stock of loans
	v[6]=-v[2]+v[5];												//central bank loans (rescue)
}
else
{
	v[5]=v[2];
	v[6]=0;
}
WRITE("Bank_Rescue", v[6]);
RESULT(v[5])

EQUATION_DUMMY("Bank_Rescue", "Bank_Accumulated_Profits")


/**********************************************************************
Bank Credit Demand;
Dawid e Delli Gatti (2018), item 2.4.5.
*********************************************************************/


EQUATION("Bank_Demand_Loans")
// Loan demand by firms and households

	v[10]=V("bank_id");

	v[0]=0;
	CYCLES(root, cur, "SECTORS")
	{
		v[1]=0;
		CYCLES(cur, cur1, "FIRMS")
		{
			v[2]=VS(cur1, "Firm_Demand_Loans");
			v[3]=VS(cur1, "firm_bank");
			if(v[3]==v[10])
				{v[1]=v[1]+v[2];}
			else
				{v[1]=v[1];}
		}
		v[0]=v[0]+v[1];
	}
	
	v[4]=0;
	CYCLES(root, cur, "CLASSES")
	{
		v[5]=VS(cur, "Class_Demand_Loans");
		v[4]=v[4]+v[5];
	}
	v[6]=VL("Bank_Market_Share", 1);
	v[7]=v[0]+v[6]*v[4];
	
RESULT(v[7])


EQUATION("Bank_Market_Share")
// Ratio of a bank's client firms to the total number of firms. 
v[10]=V("bank_id");

	v[0]=0;
	v[1]=0;
	CYCLES(root, cur, "SECTORS")
	{
		v[2]=0;
		v[3]=0;
		CYCLES(cur, cur1, "FIRMS")
		{
			v[4]=VS(cur1, "firm_bank");
			if(v[4]==v[10])
				{
				v[2]+=1;
				v[3]+=1;
				}
			else
				{
				v[2]=v[2];
				v[3]+=1; 
				}
		}
		v[0]+=v[2];
		v[1]+=v[3];
	}
	v[5]=v[1]!=0? v[0]/v[1] : 0;

RESULT(v[5])


/*
EQUATION("Bank_Market_Share")
// Fixed market share. 
	v[0]=V("fs_number_object_banks");
	v[1]=1/v[0];
RESULT(v[1])
*/

/**********************************************************************
Credit Risk and the Interest Rate
Dawid e Delli Gatti (2018), item 2.5.1.
*********************************************************************/

/*
Equations regarding "Firm_Probability_Default", "Firm_Debt_Servicing", "Firm_Interest_Rate" and "Firm_Interest_Spread" were moved to file "fun_firm_finance.h".

Equations regarding "Class_Probability_Default", "Class_Debt_Servicing", "Class_Interest_Rate" and "Class_Interest_Spread" were moved to file "fun_classes.h". 
*/






/**********************************************************************
Supply of Loans
Dawid e Delli Gatti (2018), item 2.5.2.
*********************************************************************/

		// ECONOMIC REQUIREMENT


/*
Equations regarding "Firm_Loan_Expected_Payback" were moved to file "fun_firm_finance.h".

Equations regarding "Class_Loan_Expected_Payback" were moved to file "fun_classes.h". 
*/


		// CAPITAL REQUIREMENT

EQUATION("Bank_RWAcpad")
// Bank's risk-weighted assets for credit risk according to the standardized approach (BCBS (2019), RBC 20.6 & CRE 20 to CRE 22).

	v[10]=V("bank_id");

	v[0]=0;
	CYCLES(root, cur, "SECTORS")
	{
		v[1]=0;
		CYCLES(cur, cur1, "FIRMS")
		{
			v[2]=VS(cur1, "Firm_Stock_Loans");
			v[3]=VS(cur1, "firm_bank");
			if(v[3]==v[10])
				{v[1]=v[1]+v[2];}
			else
				{v[1]=v[1];}
		}
		v[0]=v[0]+v[1];
	}
	v[4]=V("cb_rwacpad_firm"); 

	v[5]=0;
	CYCLES(root, cur, "CLASSES")
		{
		v[6]=0;
		CYCLES(cur, cur1, "CLASS_LOANS")
			{
			v[7]=VS(cur1, "class_loan_total_amount");
			v[8]=VS(cur1, "class_loan_bank_id");
			
			if(v[8]==v[10])
				{v[6]=v[6]+v[7];}
			else
				{v[6]=v[6];}
			}
		v[5]=v[5]+v[6];	
		}
	v[9]=V("cb_rwacpad_retail");
	v[11]=(v[0]*v[4])+(v[5]*v[9]);	
RESULT(v[11])

/*
EQUATION("Bank_RWAcpad")
// Alternative when class effective loans = class demand for loans.

	v[10]=V("bank_id");

	v[0]=0;
	CYCLES(root, cur, "SECTORS")
	{
		v[1]=0;
		CYCLES(cur, cur1, "FIRMS")
		{
			v[2]=VS(cur1, "Firm_Stock_Loans");
			v[3]=VS(cur1, "firm_bank");
			if(v[3]==v[10])
				{v[1]=v[1]+v[2];}
			else
				{v[1]=v[1];}
		}
		v[0]=v[0]+v[1];
	}
	v[4]=V("cb_rwacpad_firm"); 

	v[5]=0;
	CYCLES(root, cur, "CLASSES")
		{
		v[6]=VS(cur, "Class_Stock_Loans");
		v[5]=v[5]+v[6];
		}
	v[7]=V("Bank_Market_Share"); 	
	v[8]=V("cb_rwacpad_retail");
	v[9]=(v[0]*v[4])+(v[5]*v[7]*v[8]);	
RESULT(v[9])
*/

EQUATION("Bank_Supply_Capital_Classes")
// Capital supply for household loans (remaining capital supply is later directed to firm's loans). Adapted from van der Hoog & Dawid (2016), item 3.11.8.

	v[0]=V("cb_minimum_capital_ratio");										// (Parameter) Basel Capital Ratio
	v[1]=VL("Bank_Accumulated_Profits", 1);								// Proxy for bank's Capital
	v[2]=VL("Bank_RWAcpad", 1);											
	v[3]=(v[1]/v[0])-v[2];
	if (v[3]>0)
	{WRITE("bank_ration_credit", 0);}									
	else
	{WRITE("bank_ration_credit", 1);}											// (Parameter) For analysis purposes
RESULT(v[3])

EQUATION_DUMMY ("bank_ration_credit", "Bank_Supply_Capital")

	
		// RATIONING

EQUATION("Bank_Loans_Effective_Classes")
// Bank decision rule - for classes. MMM time steps require that classes disposable income is determined prior to firm requests for loans. 

	v[10]=V("bank_id");
	v[11]=V("Bank_Supply_Capital_Classes"); 
	v[12]=VL("Bank_Market_Share", 1);
	v[13]=V("cb_rwacpad_retail"); 			// ** optimization possible! **
	v[14]=V("fs_lambda");				// ** optimization possible! **

	v[0]=0;	
	v[1]=1;
	SORT("CLASSES", "Class_Probability_Default", "UP");
	CYCLES(root, cur, "CLASSES")
		{
		v[2]=VS(cur, "Class_Demand_Loans");
		v[3]=v[2]*v[12];								// Loans requested to a particular bank, given its (lagged) market share)
		v[4]=VS(cur, "Class_Interest_Rate");
		v[5]=VS(cur, "Class_Loan_Expected_Payback");
		if(v[2]>0 && v[0]+v[3]*v[13] <= v[11] && v[5] > 1)					// If the class has requested a loan & the bank has sufficient capital to cover the regulatory requirements that would be applicable to the requested loan & the prospective loan is considered profitable  ...
				{
				cur1 = ADDOBJS(cur, "CLASS_LOANS");					// ... the bank underwrites the requested loan.
					WRITES(cur1, "class_loan_total_amount", v[3]);
					WRITES(cur1, "class_loan_interest_rate", v[4]);
					WRITES(cur1, "class_loan_fixed_amortization", (v[3]/v[14]));
					WRITES(cur1, "class_loan_bank_id", v[10]);
					WRITES(cur1, "class_loan_fixed_object", 0);			

				v[0]+=v[3];						
				v[1]+=v[3]*v[13];				
				}
		else
				{
				v[0]=v[0];				
				v[1]=v[1];
				}
		}
	v[6]=v[11]-v[1];
	v[7]=max(0, v[6]);

	WRITE("Bank_Supply_Capital_Firms", v[7]);
RESULT(v[0])


/*
EQUATION("Bank_Loans_Effective_Classes")
// Alternative when class effective loans = class demand for loans.

	v[10]=V("bank_id");
	v[11]=V("cb_rwacpad_retail");
	v[12]=V("Bank_Supply_Capital_Classes");

	v[1]=0;
	CYCLES(root, cur, "CLASSES")
	{
		v[2]=VS(cur, "Class_Demand_Loans");
		v[1]=v[1]+v[2];
	}
	v[3]=V("Bank_Market_Share");
	v[4]=v[1]*v[3];

	v[5]=v[4]*v[11];
	v[6]=v[12]-v[5];
	v[7]=max(0, v[7]);
	WRITE("Bank_Supply_Capital_Firms", v[7]);
RESULT(v[4])
*/


EQUATION_DUMMY("Bank_Supply_Capital_Firms", "Bank_Loans_Effective_Class")


EQUATION("Bank_Loans_Effective_Firms")
// Bank decision rule for firm loan requests - after loans to classes. 

	v[10]=V("bank_id");
	v[11]=V("Bank_Supply_Capital_Firms"); 		
	v[12]=V("cb_rwacpad_firm"); 			
	v[13]=V("fs_lambda");
	
	v[0]=0;
	v[1]=0;
	CYCLES(root, cur, "SECTORS")
	{
		v[2]=0;
		v[3]=0;
		SORT("FIRMS", "Firm_Probability_Default", "UP");
		CYCLES(cur, cur1, "FIRMS")
		{
			v[4]=VS(cur1, "Firm_Demand_Loans");
			v[5]=VS(cur1, "Firm_Interest_Rate");
			v[6]=VS(cur1, "Firm_Desired_Investment_Expenses");
			v[7]=VS(cur1, "Firm_Loan_Expected_Payback");
			v[8]=VS(cur1, "firm_bank");
			if(v[4] > 0 && v[8]==v[10] && v[7] > 1 && v[3]+v[4]*v[12] <= v[11]) 	// If the firm requests a loan && is a client of the bank & the prospective loan is considered profitable & the bank has sufficient capital to cover regulatory requirements ...
				{
					WRITES(cur1, "firm_effective_loans", v[4]);
					cur2 = ADDOBJS(cur1, "FIRM_LOANS");			// ... the bank underwrites the requested loan.
					WRITES(cur2, "firm_loan_total_amount", v[4]);
					WRITES(cur2, "firm_loan_fixed_object", 0);
					if(v[6]!=0)
						{
						WRITES(cur2, "firm_loan_fixed_amortization", (v[4]/v[13]));
						WRITES(cur2, "id_firm_loan_long_term", 1);
						WRITES(cur2, "firm_loan_interest_rate", v[5]);
						}
					else
						{
						WRITES(cur2, "firm_loan_fixed_amortization", (v[4]/v[13]));
						WRITES(cur2, "id_firm_loan_short_term", 1);
						WRITES(cur2, "firm_loan_interest_rate", v[5]);
						}

					v[2]+=v[4];
					v[3]+=v[4]*v[12];
	
				}
			else
				{
				
					WRITES(cur1, "firm_effective_loans", 0);
					v[2]=v[2];
					v[3]=v[3];

				}
		}
		v[0]+=v[2];
		v[1]+=v[3];
	}

	v[20]=v[11]-v[1];
	v[21]=max(0, v[20]);
	WRITE("Bank_Supply_Capital", v[21]);
RESULT(v[0])


EQUATION_DUMMY("Bank_Supply_Capital", "Bank_Loans_Effective_Firms")

EQUATION("Bank_Loans_Effective")
	v[0]=V("Bank_Loans_Effective_Classes");
	v[1]=V("Bank_Loans_Effective_Firms");
	v[2]=v[0]+v[1];
RESULT(v[2])



/**********************************************************************
Bank Analysis

*********************************************************************/


EQUATION("Bank_Demand_Met")
	v[0]=V("Bank_Loans_Effective");
	v[1]=V("Bank_Demand_Loans");
	v[2]=v[1]!=0? v[0]/v[1] : 0;
RESULT(v[2])


EQUATION("Bank_Leverage")
// Total Stock of loans over total equity  ** not DEBT, as originally stated **
	v[0]=V("Bank_Total_Stock_Loans");
	v[1]=V("Bank_Accumulated_Profits");
	v[2]= v[1]!=0? v[0]/v[1] : 0;
RESULT(v[2])


EQUATION("Bank_Effective_Capital_Ratio");
//Current bank accumulated profits over risk weighted assets  ** not LOANS, as originally stated **
	v[0]=V("Bank_Accumulated_Profits");
	v[1]=V("Bank_RWAcpad");
	v[2]= v[1]!=0? v[0]/v[1] : 0;
RESULT(v[2])


EQUATION("Bank_Interest_Rate")
// Weighted Avg of Interest Rates that (would be charged) to clients
	v[10]=V("bank_id");

	v[0]=0;
	CYCLES(root, cur, "SECTORS")
	{
		v[1]=0;
			CYCLES(cur, cur1, "FIRMS")
			{
			v[2]=VS(cur1, "Firm_Stock_Loans");
			v[3]=VS(cur1, "Firm_Interest_Rate");
			v[4]=VS(cur1, "firm_bank");
			if(v[4]==v[10])
				{v[1]+=v[2]*v[3];}
			else
				{v[1]=v[1];}
			}
		v[0]+=v[1];
	}

	v[5]=0;
	CYCLES(root, cur, "CLASSES")
	{
		v[6]=VS(cur, "Class_Stock_Loans");
		v[7]=VS(cur, "Class_Interest_Rate");		
		v[5]+=v[6]*v[7];
	}
	v[8]=VL("Bank_Market_Share", 1);										// ** assess whether to include lags or not
	v[9]=V("Bank_Total_Stock_Loans");
	v[11]=v[0]+(v[5]*v[8]);
	if (v[9]>0)
		{
		v[12]=v[11]/v[9];
		}	
	else
		{
		v[12]=V("Central_Bank_Basic_Interest_Rate");
		}	
	WRITE("Bank_Interest_Rate_Short_Term", v[12]);
	WRITE("Bank_Interest_Rate_Long_Term", v[12]);
RESULT(v[12])

EQUATION_DUMMY("Bank_Interest_Rate_Short_Term", "Bank_Interest_Rate")

EQUATION_DUMMY("Bank_Interest_Rate_Long_Term", "Bank_Interest_Rate")





// ^~^~^~ Unedited equations from MMM_v3.9, as coded by Matheus Vianna ^~^~^~


/**********************************************************************
Bank Stocks and Flows - Collection from bank's clients
*********************************************************************/


EQUATION("Bank_Stock_Loans_Long_Term")
/*
Total Stock of short term loans, firms and classes
*/

	v[10]=V("bank_id");

	v[0]=0;
	CYCLES(root, cur, "SECTORS")
	{
		v[1]=0;
		CYCLES(cur, cur1, "FIRMS")
		{
			v[2]=VS(cur1, "Firm_Stock_Loans_Long_Term");
			v[3]=VS(cur1, "firm_bank");
			if(v[3]==v[10])
				v[1]=v[1]+v[2];
			else
				v[1]=v[1];
		}
		v[0]=v[0]+v[1];
	}
RESULT(v[0])


EQUATION("Bank_Total_Stock_Loans")
/*
Total Stock of loans
*/
	v[0]=V("Bank_Stock_Loans_Long_Term");
	v[1]=V("Bank_Stock_Loans_Short_Term");
	v[2]=v[0]+v[1];
RESULT(v[2])


EQUATION("Bank_Debt_Payment")
/*
Total interest payment from firms and classes
*/
	v[10]=V("bank_id");

	v[0]=0;
	CYCLES(root, cur, "SECTORS")
	{
		v[1]=0;
		CYCLES(cur, cur1, "FIRMS")
		{
			v[2]=VS(cur1, "Firm_Debt_Payment");
			v[3]=VS(cur1, "firm_bank");
			if(v[3]==v[10])
				v[1]=v[1]+v[2];
			else
				v[1]=v[1];
		}
		v[0]=v[0]+v[1];
	}
	
	v[4]=0;
	CYCLES(root, cur, "CLASSES")
	{
		v[5]=VS(cur, "Class_Debt_Payment");
		v[4]=v[4]+v[5];
	}
	v[6]=V("Bank_Market_Share");
	v[7]=v[0]+v[6]*v[4];
	
RESULT(v[7])


/**********************************************************************
Bank Profits
*********************************************************************/


EQUATION("Bank_Defaulted_Loans");
// Current bank defaulted loans    ** como essa equação funciona?? ** 

	v[0]=V("bank_defaulted_loans_temporary");
	WRITE("bank_defaulted_loans_temporary", 0);
RESULT(v[0])


EQUATION("Bank_Accumulated_Defaulted_Loans");
/*
Current bank defaulted loans 
*/
	v[0]=VL("Bank_Accumulated_Defaulted_Loans",1);
	v[1]=V("Bank_Defaulted_Loans");
	v[2]=v[0]+v[1];
RESULT(v[2])


/**********************************************************************
Bank Analysis
*********************************************************************/



EQUATION("Bank_Short_Term_Rate");
/*
Share of short term loans over total loans
*/
	v[0]=V("Bank_Stock_Loans_Short_Term");
	v[1]=V("Bank_Total_Stock_Loans");
	v[2]= v[1]!=0? v[0]/v[1] : 0;
RESULT(v[2])


EQUATION("Bank_Default_Share");
/*
Current bank defaulted loans over stock of long term loans
*/
	v[0]=V("Bank_Defaulted_Loans");
	v[1]=V("Bank_Stock_Loans_Long_Term");
	v[2]= v[1]!=0? v[0]/v[1] : 0;
RESULT(min(1,v[2]))


EQUATION("Bank_Effective_Profit_Rate");
/*
Effective profit rate on total loans
*/
	v[0]=V("Bank_Profits");
	v[1]=V("Bank_Total_Stock_Loans");
	v[2]= v[1]!=0? v[0]/v[1] : 0;
RESULT(v[2])





