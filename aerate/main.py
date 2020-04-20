import os
import re
import textwrap
import unicodedata

from lxml import etree

SCRIPT_ROOT = os.path.dirname(os.path.realpath(__file__))

with open(os.path.join(SCRIPT_ROOT, "xml", "access_8h.xml")) as file:
    document = etree.parse(file)

indent = 0

def emit(*args, sep=" ", **kwargs):
    if sep is None:
        sep = " "
    text = sep.join(args)
    text = textwrap.indent(text, " " * indent)
    print(text, **kwargs)

for function in document.xpath(r'//memberdef[@kind="function"]'):
    (definition,) = function.xpath("./definition")
    (argsstring,) = function.xpath("./argsstring")

    print(f".. c:function:: {definition.text}{argsstring.text}")

    (briefdescription,) = function.xpath("./briefdescription")

    paras = []
    for para in briefdescription.xpath("./para"):
        paras += [render_para(para)]
    emit("\n\n".join(paras), end="\n\n")

    (detaileddescription,) = function.xpath("./detaileddescription")
    paras = []
    for para in detaileddescription.xpath("./para"):
        paras += [render_para(para)]
    emit("\n\n".join(paras))

    # print(function.get("id"))
