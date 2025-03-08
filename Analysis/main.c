#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
const int ALPHABET_SIZE = 26;
const int TOP_N = 3;

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
    return 1 - p / (p1 * p2);
}

char *shift_text(const char *text, int shift)
{
    const int n = strlen(text);
    char *shifted_text = (char *)malloc((n + 1) * sizeof(char));
    for (int i = 0; i <= n; i++)
    {
        if (text[i] >= 'a' && text[i] <= 'z')
        {
            shifted_text[i] = (text[i] - 'a' + shift + 26) % 26 + 'a';
        }
        else if (text[i] >= 'A' && text[i] <= 'Z')
        {
            shifted_text[i] = (text[i] - 'A' + shift + 26) % 26 + 'A';
        }
        else
        {
            shifted_text[i] = text[i];
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
        char *shifted_text = shift_text(text, -i);
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

void output_text(FILE *fout, char *text)
{
    fprintf(fout, "%s\n", text);
    fclose(fout);
}

int main()
{
    const int TEXTS = 17;
    double normal_distribution[ALPHABET_SIZE] = {0};
    read_distrubutions("../distributions.txt", normal_distribution);
    char texts[TEXTS][1000];
    FILE *fin = fopen("sample.in", "r");
    for (int i = 0; i < TEXTS; i++)
    {
        fgets(texts[i], 1000, fin);
    }
    fclose(fin);
    FILE *fout = fopen("sample.out", "w");
    for (int i = 0; i < TEXTS; i++)
    {
        int shift = rand() % 26;
        fprintf(fout, "Text: %s\n", texts[i]);
        fprintf(fout, "Shift: %d\n", shift);
        char *shifted_text = shift_text(texts[i], shift);

        int top_shifts[TOP_N];
        double top_distances[TOP_N];
        break_caesar_cipher(shifted_text, normal_distribution, top_shifts, top_distances, chi_squared_distance);
        fprintf(fout, "\n");
        fprintf(fout, "Chi Squared Distance\n");
        for (int j = 0; j < TOP_N; j++)
        {
            fprintf(fout, "\n");
            fprintf(fout, "Top %d: Shift = %d, Distance = %lf\n", j + 1, top_shifts[j], top_distances[j]);
            char *decrypted_text = shift_text(shifted_text, -top_shifts[j]);
            fprintf(fout, "Decrypted text: %s\n", decrypted_text);
            free(decrypted_text);
        }
        break_caesar_cipher(shifted_text, normal_distribution, top_shifts, top_distances, euclidean_distance);
        fprintf(fout, "\n");
        fprintf(fout, "Euclidean Distance\n");
        for (int j = 0; j < TOP_N; j++)
        {
            fprintf(fout, "\n");
            fprintf(fout, "Top %d: Shift = %d, Distance = %lf\n", j + 1, top_shifts[j], top_distances[j]);
            char *decrypted_text = shift_text(shifted_text, -top_shifts[j]);
            fprintf(fout, "Decrypted text: %s\n", decrypted_text);
            free(decrypted_text);
        }
        break_caesar_cipher(shifted_text, normal_distribution, top_shifts, top_distances, cosine_distance);
        fprintf(fout, "\n");
        fprintf(fout, "Cosine Distance\n");
        for (int j = 0; j < TOP_N; j++)
        {
            fprintf(fout, "\n");
            fprintf(fout, "Top %d: Shift = %d, Distance = %lf\n", j + 1, top_shifts[j], top_distances[j]);
            char *decrypted_text = shift_text(shifted_text, -top_shifts[j]);
            fprintf(fout, "Decrypted text: %s\n", decrypted_text);
            free(decrypted_text);
        }
        fprintf(fout, "\n");
        free(shifted_text);
    }
    fclose(fout);
}