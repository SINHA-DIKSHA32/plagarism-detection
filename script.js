
// Save input.txt
function checkPlagiarism() {
  const text = document.getElementById("InputText").value.trim();
  if (!text) {
    document.getElementById("plagiarismResult").innerText = "⚠ Please enter some text!";
    return;
  }

  const blob = new Blob([text], { type: "text/plain" });
  const url = window.URL.createObjectURL(blob);
  const a = document.createElement("a");
  a.href = url;
  a.download = "input.txt";
  a.style.display = "none";
  document.body.appendChild(a);
  a.click();
  document.body.removeChild(a);

  document.getElementById("plagiarismResult").innerText =
    "✅ input.txt saved. Now run backend (checker.exe).";
}

// Count words
function countWord() {
  const text = document.getElementById("InputText").value.trim();
  const wordCount = text.length > 0 ? text.split(/\s+/).length : 0;
  document.getElementById("wordCountDisplay").innerText = `Total Words: ${wordCount}`;
}

// Grammar Check (basic)
function checkGrammar() {
  const text = document.getElementById("InputText").value.trim();
  let issues = [];

  if (!text) {
    document.getElementById("grammarResult").innerText = "⚠ No text entered!";
    return;
  }

  const sentences = text.split(/(?<=[.?!])\s+/);
  sentences.forEach((s, i) => {
    const trimmed = s.trim();
    if (!trimmed.match(/[.?!]$/)) {
      issues.push(`Sentence ${i + 1}: Missing punctuation at end.`);
    }
    const words = trimmed.toLowerCase().split(/\s+/);
    for (let j = 1; j < words.length; j++) {
      if (words[j] === words[j - 1]) {
        issues.push(`Sentence ${i + 1}: Repeated word "${words[j]}"`);
      }
    }
  });

  document.getElementById("grammarResult").innerText =
    issues.length > 0 ? issues.join("\n") : "✅ No grammar issues found.";
}

/* Show result.txt (only works via Live Server)
function showResultTxt() {
  fetch("result.txt")
    .then((res) => res.text())
    .then((data) => {
      document.getElementById("resultText").innerText = data;
    })
    .catch(() => {
      document.getElementById("resultText").innerText =
        "❌ result.txt not found. Run checker.exe first or upload manually.";
    });
}*/

// Manual Upload result.txt
document.getElementById("fileUpload").addEventListener("change", function () {
  const file = this.files[0];
  if (file) {
    const reader = new FileReader();
    reader.onload = function (e) {
      document.getElementById("resultText").innerText = e.target.result;
    };
    reader.readAsText(file);
  }
});
