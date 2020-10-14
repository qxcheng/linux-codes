import torch
import random
from config import *
from data import tensorFromSentence, TrainDatasets
from model import *


def evaluate(encoder, decoder, sentence, train_data, max_length=MAX_LENGTH):
    with torch.no_grad():
        input_tensor = tensorFromSentence(train_data.input_lang, sentence)
        input_length = input_tensor.size()[0]
        encoder_hidden = encoder.initHidden()

        encoder_outputs = torch.zeros(max_length, encoder.hidden_size, device=device)

        for ei in range(input_length):
            encoder_output, encoder_hidden = encoder(input_tensor[ei],
                                                     encoder_hidden)
            encoder_outputs[ei] += encoder_output[0, 0]

        decoder_input = torch.tensor([[SOS_token]], device=device)  # SOS

        decoder_hidden = encoder_hidden

        decoded_words = []
        decoder_attentions = torch.zeros(max_length, max_length)

        for di in range(max_length):
            decoder_output, decoder_hidden, decoder_attention = decoder(
                decoder_input, decoder_hidden, encoder_outputs)
            decoder_attentions[di] = decoder_attention.data
            topv, topi = decoder_output.data.topk(1)
            if topi.item() == EOS_token:
                decoded_words.append('<EOS>')
                break
            else:
                decoded_words.append(train_data.output_lang.index2word[topi.item()])

            decoder_input = topi.squeeze().detach()

        return decoded_words, decoder_attentions[:di + 1]

def evaluateRandomly(encoder, decoder, train_data, n=10):
    for i in range(n):
        pair = random.choice(train_data.pairs)
        print('>', pair[0])
        print('=', pair[1])
        output_words, attentions = evaluate(encoder, decoder, pair[0], train_data)
        output_sentence = ' '.join(output_words)
        print('<', output_sentence)
        print('')

if __name__ ==  '__main__':
    train_data = TrainDatasets()

    hidden_size = 256
    encoder1 = EncoderRNN(train_data.input_lang.n_words, hidden_size).to(device)
    attn_decoder1 = AttnDecoderRNN(hidden_size, train_data.output_lang.n_words, dropout_p=0.1).to(device)

    encoder1.load_state_dict(torch.load("encoder.pth"))
    attn_decoder1.load_state_dict(torch.load("decoder.pth"))

    evaluateRandomly(encoder1, attn_decoder1, train_data)
