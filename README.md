# Competitive Programming Library

競技プログラミング用の C++ ライブラリです。ヘッダは `C++/` 以下にあります。

## 収録内容

- データ構造: Binary Indexed Tree、Binary Trie、Trie、Interval Set、Sparse Table、Disjoint Sparse Table、Sqrt Tree
- Segment Tree: 通常、遅延、双対、動的、永続
- Union-Find: 通常、Undo 対応
- 文字列: Suffix Array、LCP Array、Z Algorithm
- 数学: ModInt、ModPow、組合せ、約数列挙、素因数分解、素数篩
- その他: Mo、Slope Trick、Weighted Slope Trick、提出用テンプレート

## AtCoder で使用する場合

[AtCoder 生成AI対策ルール（2026年10月14日版）](https://info.atcoder.jp/entry/llm-rules-ja)では、コンテスト開始前に生成AIで作成したコードを使用する場合、開始前の Web 公開と、提出コードへの参照 URL の添付が必要です。

このライブラリをコピーして提出する際は、ソースコードに次のコメントを含めてください。

```cpp
// Library: https://github.com/lazy-plus1/library
```

使用するバージョンがコンテスト開始前に公開されていることを確認してください。更新履歴は GitHub のコミット履歴から確認できます。

## 更新方法

このディレクトリで変更をコミットし、GitHub に push します。

```sh
git add C++ README.md .gitignore
git commit -m "Update library"
git push
```
