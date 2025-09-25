param(
    [Parameter(Mandatory=$true)]
    [string]$FilePath
)

# Extract class name from file name (without extension)
$ClassName = [System.IO.Path]::GetFileNameWithoutExtension($FilePath)

# Function to convert CamelCase → UPPER_SNAKE_CASE
function ToUpperSnakeCase($name) {
    $result = ""
    for ($i = 0; $i -lt $name.Length; $i++) {
        $char = $name[$i]
        
        if ($i -gt 0) {
            $prev = $name[$i - 1]
            $next = if ($i + 1 -lt $name.Length) { $name[$i + 1] } else { '' }

            if ((($prev -ge 'a' -and $prev -le 'z') -or ($prev -ge '0' -and $prev -le '9')) -and ($char -ge 'A' -and $char -le 'Z')) {
                $result += "_"
            }
            elseif (($prev -ge 'A' -and $prev -le 'Z') -and ($char -ge 'A' -and $char -le 'Z') -and ($next -ge 'a' -and $next -le 'z')) {
                $result += "_"
            }
        }

        $result += $char
    }

    return $result.ToUpper() + "_H"
}

$IncludeGuard = ToUpperSnakeCase $ClassName
$Destination = $FilePath  # write into the same file

$Content = @"
#pragma once
#ifndef $IncludeGuard
#define $IncludeGuard

class $ClassName {
public:
    $ClassName() = default;
    ~$ClassName() = default;

protected:

private:
};

#endif // $IncludeGuard
"@

$Content | Set-Content $Destination -Encoding UTF8
Write-Host "Created header: $Destination with guard $IncludeGuard"
