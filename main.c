#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
const int ALPHABET_SIZE = 26;
const int TOP_N = 3;

void clrscr()
{
    system("@cls||clear");
}

void read_distrubutions(const char *filename, double distribution[ALPHABET_SIZE])
{
    FILE *fin = fopen(filename, "r");
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        fscanf(fin, "%lf", &distribution[i]);
    }
    fclose(fin);
}

void compute_histogram(const char *text, double distribution[ALPHABET_SIZE])
{
    int letters = 0;
    int n = strlen(text);
    for (int i = 0; i < n; i++)
    {
        if (text[i] >= 'a' && text[i] <= 'z')
        {
            distribution[(text[i] - 'a')]++;
            letters++;
        }
        else if (text[i] >= 'A' && text[i] <= 'Z')
        {
            distribution[(text[i] - 'A')]++;
            letters++;
        }
    }
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        distribution[i] = distribution[i] / letters * 100;
    }
}

double chi_squared_distance(const double histogram1[ALPHABET_SIZE], const double histogram2[ALPHABET_SIZE])
{
    double chi = 0;
    for (int i = 0; i < 26; i++)
    {
        double delta = histogram2[i] - histogram1[i];
        chi += delta * delta / histogram1[i];
    }
    return chi;
}

double euclidean_distance(const double histogram1[ALPHABET_SIZE], const double histogram2[ALPHABET_SIZE])
{
    double e = 0;
    for (int i = 0; i < 26; i++)
    {
        double delta = histogram2[i] - histogram1[i];
        e += delta * delta;
    }
    return sqrt(e);
}

double cosine_distance(const double histogram1[ALPHABET_SIZE], const double histogram2[ALPHABET_SIZE])
{
    double p = 0, p1 = 0, p2 = 0;
    for (int i = 0; i < 26; i++)
    {
        p += histogram1[i] * histogram2[i];
        p1 += histogram1[i] * histogram1[i];
        p2 += histogram2[i] * histogram2[i];
    }
    p1 = sqrt(p1);
    p2 = sqrt(p2);
    return p / (p1 * p2);
}

char *shift_text(const char *text, int shift)
{
    const int n = strlen(text);
    char *shifted_text = (char *)malloc((n + 1) * sizeof(char));
    for (int i = 0; i <= n; i++)
    {
        if (text[i] >= 'a' && text[i] <= 'z')
        {
            shifted_text[i] = (text[i] - 'a' + shift) % 26 + 'a';
        }
        else if (text[i] >= 'A' && text[i] <= 'Z')
        {
            shifted_text[i] = (text[i] - 'A' + shift) % 26 + 'A';
        }
    }
    return shifted_text;
}

void break_caesar_cipher(const char *text, const double normal_distribution[ALPHABET_SIZE], int top_shifts[TOP_N], double top_distances[TOP_N], double (*distance_function)(const double[], const double[]))
{
    struct str
    {
        int shift;
        double distance;
    } shifts[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        char *shifted_text = shift_text(text, i);
        double distribution[ALPHABET_SIZE] = {0};
        compute_histogram(shifted_text, distribution);

        shifts[i].shift = i;
        shifts[i].distance = distance_function(normal_distribution, distribution);
    }
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        for (int j = i + 1; j < ALPHABET_SIZE; j++)
        {
            if (shifts[i].distance > shifts[j].distance)
            {
                struct str temp = shifts[i];
                shifts[i] = shifts[j];
                shifts[j] = temp;
            }
        }
    }
    for (int i = 0; i < TOP_N; i++)
    {
        top_shifts[i] = shifts[i].shift;
        top_distances[i] = shifts[i].distance;
    }
}

void read_text_terminal(char *text)
{
    fgets(text, 1000, stdin);
    text[strlen(text) - 1] = '\0';
}

void read_text_file(FILE *fin, char *text)
{
    int i = 0;
    char c;
    do
    {
        c = fgetc(fin);
        text[i++] = c;
    } while (c != EOF);
    text[i - 1] = '\0';
    fclose(fin);
}

void output_text(FILE *fout, char *text)
{
    fprintf(fout, "%s\n", text);
    fclose(fout);
}

void menu()
{
    char txt[1000] = "";
    while (1)
    {
        if (txt[0] != '\0')
        {
            printf("Your current text is: \"%s\"\n", txt);
        }
        else
        {
            printf("You have not entered any text yet.\n");
        }

        printf("1. Read text from keyboard\n");
        printf("2. Read text from file\n");
        printf("q. Quit\n");
        printf("Enter choice (please only write one letter): ");
        char o;
        scanf("%c", &o);
        switch (o)
        {
        case '1':
            printf("Enter text: ");
            fflush(stdin);
            read_text_terminal(txt);
            break;

        case '2':
            printf("Enter file name: ");
            fflush(stdin);
            char filename[100];
            read_text_terminal(filename);
            FILE *fin = fopen(filename, "r");
            if (fin == NULL)
            {
                printf("File not found\n");
            }
            else
            {
                read_text_file(fin, txt);
            }
            break;

        case 'q':
        case 'Q':
            exit(0);
            break;

        default:
            printf("Invalid option. Try again");
            break;
        }
    }
}

int main()
{
    menu();
}