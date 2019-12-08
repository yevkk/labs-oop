#ifndef PREVIEWHIGHLIGHTER_H
#define PREVIEWHIGHLIGHTER_H


#include<QSyntaxHighlighter>
#include<QRegularExpression>

class PreviewHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    enum Construct {
        Entity,
        Node
    };

    PreviewHighlighter(QTextDocument *document);

    void setFormatFor(Construct construct, const QTextCharFormat &format);


    QTextCharFormat formatFor(Construct construct) const
    { return m_formats[construct]; }

protected:
    enum State {
        NormalState = -1,
        InNode
    };

    void highlightBlock(const QString &text) override;

private:
    QTextCharFormat m_formats[2];
};

#endif // PREVIEWHIGHLIGHTER_H
