#include "previewhighlighter.h"

void PreviewHighlighter::setFormatFor(Construct construct, const QTextCharFormat &format) {
    m_formats[construct] = format;
    rehighlight();
}

PreviewHighlighter::PreviewHighlighter(QTextDocument *document)
        : QSyntaxHighlighter(document)
    {
        QTextCharFormat entityFormat;
        entityFormat.setForeground(QColor(255, 255, 255));
        entityFormat.setFontWeight(QFont::Bold);
        setFormatFor(Entity, entityFormat);

        QTextCharFormat nodeFormat;
        nodeFormat.setForeground(QColor(10, 30, 160));
        nodeFormat.setFontWeight(QFont::Bold);
        setFormatFor(Node, nodeFormat);
    }

void PreviewHighlighter::highlightBlock(const QString &text) {
    int state = previousBlockState();
    int len = text.length();
    int start = 0;
    int pos = 0;
    while (pos < len) {
        switch (state) {
        case NormalState:
        default:
            while (pos < len) {
                QChar ch = text.at(pos);
                if (ch == '<') {
                    state = InNode;
                    break;
                } else if (ch == '&') {
                    start = pos;
                    while (pos < len
                           && text.at(pos++) != ';');
                    setFormat(start, pos - start,
                              m_formats[Entity]);
                } else {
                    pos++;
                }
            }
            break;
        case InNode:
            QChar quote = QChar::Null;
            start = pos;
            while (pos < len) {
                QChar ch = text.at(pos);
                if (quote.isNull()) {
                    if (ch == '\\' || ch == '\"') {
                        quote = ch;
                    } else if (ch == '>') {
                        pos++;
                        state = NormalState;
                        break;
                    }
                } else if (ch == quote) {
                    quote = QChar::Null;
                }
                pos++;
            }
            setFormat(start, pos - start, m_formats[Node]);
        }
    }
}
