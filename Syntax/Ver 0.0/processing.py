import re, json, os, sys, shlex
from shutil import copyfile
from subprocess import call
from ru_syntax_utils import segment, posttok, posttag

mystem_options = [
        '--format=json', # plaintext at input, json at output
        '-i', # print grammar tags
        '-d', # use lexical disambiguation
        '-c', # copy input onto output
        '--eng-gr', # grammar tags in English
    ]


def process(ifname, ofname, app_root, mystem_path, 
    treetagger_bin, treetagger_par,
    mfname_i, mfname_o, ttfname_i, ttfname_o, raw_fname):
    """
    Process text file.
    """
    with open(ifname, 'r', encoding='utf-8') as ifile:
        raw_text = ifile.readlines()

    # segmentation
    segmented_text = segment.segment_text(raw_text)
    with open(mfname_i, 'w', encoding='utf-8') as tmp_file:
        tmp_file.write(segmented_text)

    # mystem
    mystem_call_list = [mystem_path] + mystem_options + [mfname_i, mfname_o]
    call(mystem_call_list)
    with open(mfname_o, 'r', encoding='utf-8') as tmp_file:
        text = tmp_file.read()
        analyzed = json.loads(text.strip())

    # post-mystem correction
    sentences = posttok.mystem_postprocess(analyzed)
    tt_tokens = posttag.treetagg(sentences, ttfname_i, ttfname_o, 
                                        treetagger_bin, treetagger_par)
    sentences = posttag.tt_correct(sentences, tt_tokens)
    with open(raw_fname, 'w', encoding='utf-8') as raw_file:
        segment.flush(sentences, raw_file)

    return 0
