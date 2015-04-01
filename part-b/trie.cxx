#include <fstream>
#include <iterator>
#include <iostream>
#include "trie.hxx"
#include <queue>

using namespace std;
//using namespace cxx_project;

template <typename OutIter>
size_t trie::output_matches(string const& pattern, OutIter&& out) const
{
  size_t retval{};

  queue <pair <string, trie const*>> bfs;

  size_t cur_prefix_len {};
  bfs.emplace(string{},this);
  for (auto g : pattern){
    auto curchar = g;
	//cout << curchar;
    while (!bfs.empty() && bfs.front().first.size() == cur_prefix_len)
    {
		//cout << "first while";
      auto node = bfs.front();
      if (node.second!=nullptr)
      {
        auto child = node.second->children.find(curchar);
        auto childEnd = children.end ();
        if (curchar !='?')
        {
		//	cout << "first if ";
          if (child!=childEnd){
            bfs.emplace(node.first + curchar, child->second);
          }
        }
        else if (curchar == '?')
        {
          for (auto& child : node.second->children)
          {
			//  cout << "for 2 ";
            bfs.emplace (node.first + child.first, child.second);
          }
        }
      }
      bfs.pop();
    }
	cur_prefix_len++;
  }

  while (!bfs.empty()){
	  //cout << "second while";
    auto node = bfs.front ();
    bool is_match = false;

    if (node.first.size () == pattern.size() && node.second != nullptr){
      auto i = node.second->children.find(char{});
      auto iEnd = node.second->children.end();

      if (i != iEnd){
        is_match = true;
      }
      if (is_match == true){
        *out++ = node.first;
        ++retval;
      }
    }
    bfs.pop ();
  }

  return retval;
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    cerr << "Usage: " << argv[0] << " <FILENAME> <PATTERN>" << endl;
    return 1;
  }

  try
  {
    trie t;

    ifstream in(argv[1]);
    for (string s; in >> s; )
      t.add(s);

    auto count = t.output_matches(
      argv[2],
      ostream_iterator<string>(cout, "\n")
    );

    cout
      << "\n"
      << count
      << (count != 1 ? " matches were found." : " match was found.")
      << endl
    ;
  }
  catch (...)
  {
    cerr << "EXCEPTION OCCURRED! Aborting..." << endl;
  }
}
