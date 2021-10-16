#include "funtii.h"

void rezolvare()
{
    char buffer[70];//pantru citirea comenzilor
    char **oper;//datele din linia citita
    char h[100];//informatia dintr-o celula history
    int  i = 0, nr_all_card= 0;//numarul total de carduri existente in baza de date
    ListaLC L = NULL;//lista LC
    ListaLSC card = NULL;
    FILE *file = fopen( "input.in" , "r");
    FILE *out = fopen("output.out", "w");
    if (file) 
    {
       
        fgets(buffer, 70, file);//citesc numarul maxim de caruri
        int N_max = atoi(buffer);
        while(fgets(buffer, 70, file) )
        {//cat exista linii in fisier
            oper = Info(buffer);//obtin datele din buffer
            //sterg din buffer caracterul '\n'
            buffer[strlen(buffer) - 1] = '\0';

            //verific ce operatia am de facut
            if( strcmp(oper[0], "add_card") == 0)
            {//daca trebuie sa adaug un card
                if(L) card = find_card(L, oper[1], N_max);
                if(card)
                {//daca cardul deja exista
                    fprintf(out, "The card already exists\n");
                }
                else
                {
                    if(N_max == nr_all_card)
                    {//verific daca nu trebuie redimensionarea bazei de date
                        N_max *= 2;//dublez numarul maxim de carduri
                        reorganizare(L, N_max);//redistribui
                    }
                    add_card(&L, oper, N_max);//adaug cardul in baza de date
                    nr_all_card++;//incrementez numarul de carduri existente
                }
            }  

            if( strcmp(oper[0] , "show") == 0)
            {//daca trebuie sa afisez
                funtia_show(L, oper, out, N_max);//afisez
            }

            if( strcmp(oper[0], "delete_card") == 0)
            {//daca trebuie sa sterg un card
                dell_card(L, oper[1], N_max);//streg cardul
                nr_all_card--;//decrementez numarul total de carduri existente
            }

            if( strcmp(oper[0], "insert_card") == 0)
            {//daca trebuie sa inserez un card
                card = find_card(L, oper[1], N_max);//caut cardul
                if(!card)continue;
                if(card->info->block >= 3)
                {//daca are deja 3 inserari gresite
                        fprintf(out, "The card is blocked. ");
			            fprintf(out, "Please contact the administrator.\n");
                        sprintf(h, "FAIL, %s", buffer);//formez istoria
                }
                else if( strcmp(oper[2], card->info->pin) == 0 )
                {//daca pinul corespunde 
                    if(strcmp(card->info->status, "ACTIVE") != 0)
                    fprintf(out, "You must change your PIN.\n");
                    card->info->block = 0;
                    sprintf(h, "SUCCESS, %s", buffer);//formez istoria
                }
                else 
                {
                    fprintf(out, "Invalid PIN\n");//pi incorect                                      
                    card->info->block+=1;//incrementez inserarile incorecte
                    if(card->info->block >= 3)
                    {//daca cardul are deja 3 inserari gresite
                        fprintf(out, "The card is blocked. ");
			            fprintf(out, "Please contact the administrator.\n");
                        strcpy(card->info->status, "LOCKED");//blochez cardul
                    }
                    sprintf(h, "FAIL, %s", buffer);//formez istoria
                }                
                add_H( &(card->info)->history, h);//adaug instoria
                
            }
            if( strcmp(oper[0], "cancel") == 0)
            {//daca trebuie sa retrag un card
                card = find_card(L, oper[1], N_max);//caut cardul
                if(!card)continue;
               
                sprintf(h, "SUCCESS, %s", buffer);//formez istoria
                add_H( &(card->info)->history, h);//adaug istoria

            }

            if( strcmp(oper[0], "unblock_card") == 0)
            {//daca trebuie sa dezblochez un card
                card = find_card(L, oper[1], N_max);//caut cardul
                if(!card)continue;
                strcpy(card->info->status, "ACTIVE");//il fac activ
                card->info->block = 0;//resetez numarul de inserari incorecte
            }

            if( strcmp(oper[0], "pin_change") == 0)
            {//daca trebuie sa schimb pinul
                if(strlen(oper[2]) == 4 && ver(oper[2]))
                {//verific corectitudinea pinului
                    card = find_card(L, oper[1], N_max);//caut cardul
                    if(!card)continue;
                    strcpy(card->info->pin, oper[2]);//modific pinul
                    strcpy(card->info->status, "ACTIVE");//activez cardul
                    sprintf(h, "SUCCESS, %s", buffer);//formez istoria
                }
                else
                {//daca pinul nu invalid
                    fprintf(out, "Invalid PIN\n"); 
                    sprintf(h, "FAIL, %s", buffer);//formez istoria
                }
                add_H( &(card->info)->history, h);//adaug istoria
                
            }

            if( strcmp(oper[0], "balance_inquiry") == 0)
            {//daca trebuie sa consulez soldul 
                card = find_card(L, oper[1], N_max);//caut cardul
                if(!card)continue;
                fprintf(out, "%s\n", card->info->balance);//afisez soldul
                sprintf(h, "SUCCESS, %s", buffer);//formez istoria
                add_H( &(card->info)->history, h);//adaug istoria
                
            }

             if( strcmp(oper[0], "recharge") == 0)
            {//daca trebuie sa incarc contul
                if( atoi(oper[2]) % 10)
                {//verific daca suma este multiplu a lui 10
                    fprintf(out, "The added amount must be multiple of 10\n");
                    sprintf(h, "FAIL, %s", buffer);//formez istoria
                }
                else
                {
                    card = find_card(L, oper[1], N_max);//caut cardul
                    if(!card)continue;
                    int sum = atoi(oper[2])+ atoi(card->info->balance);
                    char var[20];
                    sprintf(var, "%d", sum);
                    strcpy(card->info->balance, var);//actualizez suma
                    fprintf(out, "%s\n", card->info->balance);//afisez soldul
                    sprintf(h, "SUCCESS, %s", buffer);//formez istoria
                }
                add_H( &(card->info)->history, h);//adaug istoria
                
            }
            
             if( strcmp(oper[0], "cash_withdrawal") == 0)
            {//daca trebuie sa retrag numeral
                if( atoi(oper[2]) % 10)
                {//verific daca suma este multiplu de 10
                    fprintf(out, "The requested amount must be multiple of 10");
                    fprintf(out, "\n");
                    sprintf(h, "FAIL, %s", buffer);//formez istoria
                }

                else
                {
                    card = find_card(L, oper[1], N_max);//caut cardul
                    if(!card)continue;
                    int sum = atoi(card->info->balance) - atoi(oper[2]) ;
                    char var[20];
                    if( sum < 0)
                    {//verifc resursele contului
                        fprintf(out, "Insufficient funds\n");
                        sprintf(h, "FAIL, %s", buffer);//formez istoria
                    }
                    else
                    {
                        sprintf(var, "%d", sum);
                        strcpy(card->info->balance, var);//reactualizez suma
                        if(sum == 0) strcpy(card->info->balance, "0\0");
                        fprintf(out, "%s\n", card->info->balance);
                        sprintf(h, "SUCCESS, %s", buffer);//formez istoria
                    }
                }
                add_H( &(card->info)->history, h);//adaug istoria
            }
            
            if( strcmp(oper[0], "transfer_funds") == 0)
            {//daca trebuie sa fac o tranzactie
                if( atoi(oper[3]) % 10)
                {//verific daca suma este multiplu de 10
                    fprintf(out, "The transferred amount must be multiple ");
                    fprintf(out, "of 10\n");
                    sprintf(h, "FAIL, %s", buffer);//formez istoria
                }
                else
                {
                    card = find_card(L, oper[1], N_max);//caut cardul sursa
                    if(!card)continue;
                    int sum = atoi(card->info->balance) - atoi(oper[3]);
                    char var[20];
                    if( sum < 0)
                    {//verific resursele contului
                        fprintf(out, "Insufficient funds\n");
                        sprintf(h, "FAIL, %s", buffer);//formez istoria
                    }
                    else
                    {//caur cardul destinati
                        ListaLSC dest = find_card(L, oper[2], N_max);
                        if(!dest)continue;
                        sprintf(var, "%d", sum);
                        strcpy(card->info->balance, var);//actualizez suma
                        int sum2 = atoi(oper[3])+ atoi(dest->info->balance);
                        char var2[20];
                        sprintf(var2, "%d", sum2);
                        strcpy(dest->info->balance, var2);//actualizez suma
                        if(sum == 0) strcpy(card->info->balance, "0\0");
                        fprintf(out, "%s\n", card->info->balance);//afisare sold
                        sprintf(h, "SUCCESS, %s", buffer);//formez istoria
                        add_H( &(dest->info)->history, h);//adaug istoria 
                    }
                }
               
                add_H( &(card->info)->history, h);//adaug istoria
                
            }

            if( strcmp(oper[0], "reverse_transaction") == 0)
            {//daca trebuie sa anulez o tranzactie
                //caut cardul surs si destinatia
                card = find_card(L, oper[1], N_max);
                ListaLSC dest = find_card(L, oper[2], N_max);
                if(!card || !dest)continue;
                int sum = atoi(dest->info->balance) - atoi(oper[3]) ;
                if(sum < 0)
                {//verific daca exista resurse indeajuns pe cardul destinatie
                    fprintf(out, "The transaction cannot be reversed\n");
                }
                else
                {
                    char var[20], var2[20] ;
                    int sum2 = atoi(card->info->balance) + atoi(oper[3]);
                    sprintf(var, "%d", sum);
                    strcpy(dest->info->balance, var);//actualizez suma
                    sprintf(var2, "%d", sum2);
                    strcpy(card->info->balance, var2);//actualizez suma
                    //sterg istoria din cardul destinatie
                    dell_H(&(dest->info)->history, oper[1], oper[2], oper[3]);
                    sprintf(h, "SUCCESS, %s", buffer);//formez istoria
                    add_H( &(card->info)->history, h);//adaug istoria
                }
                
            }
            while(oper[i])
            {
                free(oper[i++]);
            }
            free(oper);//eliberez operatiile
        }
    }
    free_LC(&L);//eliberez lista
    //inchid fisierele
    fclose(file);
    fclose(out);
}


int main(void)
{
    rezolvare();
    return 0;
}
